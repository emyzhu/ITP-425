#include "SrcMain.h"
#ifdef _WIN32
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <iostream>
#include "Node.h"
#include <fstream>

extern int proccparse(); // NOLINT
struct yy_buffer_state; // NOLINT
extern void procc_flush_buffer(yy_buffer_state* b); // NOLINT
extern int procclex_destroy(); // NOLINT
extern FILE* proccin; // NOLINT

extern int gLineNumber;
extern NProgram* gProgram;
bool gSyntaxError = false;

// CHANGE ANYTHING ABOVE THIS LINE AT YOUR OWN RISK!!!!

int ProcessCommandArgs(int argc, const char* argv[])
{
	gLineNumber = 1;
	gSyntaxError = false;
	if (argc < 2)
	{
		std::cout << "You must pass the input file as a command line parameter." << std::endl;
		return 1;
	}

	// Read input from the first command line parameter
	proccin = fopen(argv[1], "r");
	if (proccin == nullptr)
	{
		std::cout << "File not found: " << argv[1] << std::endl;
		return 1;
	}
	procc_flush_buffer(nullptr);

	// Start the parse
	proccparse();

	if (gProgram != nullptr && argc == 3)
	{
		// TODO: Add any needed code for Parts 2, 3, and 4!!!
        std::ofstream outFile ("ast.txt");
        if (outFile.is_open()) {
            gProgram->OutputAST(outFile, 0);
        }
        outFile.close();
//
//        if(strstr(argv[2],"emit")) {
//
//        }
        
        std::size_t found = std::string(argv[2]).find("emit");
        std::ofstream outFile2 ("emit.txt");
        if (found != std::string::npos) {
            CodeContext c;
            gProgram->CodeGen(c);
            if (outFile2.is_open()) {
//                gProgram->OutputAST(outFile2, 0);
                for(int i = 0; i < c.instructions.size(); i++) {
                    outFile2 << c.instructions[i].first << " ";
                    for (auto iter = c.instructions[i].second.begin();  iter != c.instructions[i].second.end();  ++iter) {
                        if (iter != c.instructions[i].second.begin()) {
                            outFile2 << ",";
                        }
                      outFile2 << *iter;
                    }
                    outFile2 << "\n";

                    
                }

            }
            outFile2.close();
        }
        
        std::size_t found2 = std::string(argv[2]).find("reg");
        if (found2 != std::string::npos) {
            CodeContext c;
            gProgram->CodeGen(c);
            std::ofstream outFile3 ("reg.txt");
            if (outFile3.is_open()) {
                
                std::vector<std::pair<int, int>> intervals(c.lastRegUsed);
                
                for(int i = 0; i < intervals.size(); i++) {
                    
                    // access one instruction
                    for (int j = 0; j < c.instructions.size(); ++j) {
                        
                        //access parameters for each instruction
                        for (int k = 0; k < c.instructions[j].second.size(); ++k) {
                            
                            //check if the param is equal to VR
                            if (c.instructions[j].second[k] == "%" + std::to_string(i)) {
                                
                                // if intervals are empty, first occurance of virtual register
                                if (intervals[i].first == 0 && intervals[i].second == 0) {
                                    intervals[i].first = j;
                                }
                                else {
                                    intervals[i].second = j;
                                }
                                
                            }
                            
                        }
                        
                    }
    
                }
                
                outFile3 << "INTERVALS:" << std::endl;
                    
                for (int i = 0; i < intervals.size(); ++i) {
                    outFile3 << "%" << i << ":" << intervals[i].first << "," << intervals[i].second << std::endl;
                }
                                        

                
                std::vector<int> real(c.lastRegUsed);
                
                std::vector<bool> isAvailable(c.lastRegUsed, true);
                
                
                //loop from 0 to number of instructions
                for (int i = 0; i < c.instructions.size(); ++i) {
                    for (int j = 0; j < intervals.size(); ++j) {
                        //check if anything is active in intervals during each instruction
                        if (intervals[j].first == i) {
                            //find first available register in isAvailable
//                            auto whereAvail = std::find(isAvailable.begin(), isAvailable.end(), true);
                            int idxAvail = -1;
                            for (auto it = isAvailable.begin(); it != isAvailable.end(); ++it) {
                                if (*it && idxAvail == -1) {
                                    idxAvail = std::distance(isAvailable.begin(), it);
                                }
                            }
                            
//                            auto idxAvail = std::distance(isAvailable.begin(), whereAvail);
                            isAvailable[idxAvail] = false;
                            //store location of register in real
                            real[j] = idxAvail;
                        }
                        //check if anything has expired in intervals during each instruction
                        if (intervals[j].second == i) {
                            isAvailable[real[j]] = true;
                        }
                    }
                }
                
                //replace virtual w/real
                for (int i = 0; i < c.instructions.size(); ++i) {
                    for (int j = 0; j < c.instructions[i].second.size(); ++j) {
                        
                        for (int k = 0; k < real.size(); ++k) {
                            
                            if ("%" + std::to_string(k) == c.instructions[i].second[j]) {
                                c.instructions[i].second[j] = "r" + std::to_string(real[k] + 1);
                            }
                            
                        }
                        
                    }
                }
                
                outFile3 << "ALLOCATION:" << std::endl;
                for(int i = 0; i < real.size(); i++) {
                    outFile3 << "%" << i << ":r" << real[i] + 1 << std::endl;
                }
                
                std::ofstream outFile2 ("emit.txt");
                if (outFile2.is_open()) {
                    for(int i = 0; i < c.instructions.size(); i++) {
                        outFile2 << c.instructions[i].first << " ";
                        for (auto iter = c.instructions[i].second.begin();  iter != c.instructions[i].second.end();  ++iter) {
                            if (iter != c.instructions[i].second.begin()) {
                                outFile2 << ",";
                            }
                          outFile2 << *iter;
                        }
                        outFile2 << "\n";
                    }
                }

             
            }
            outFile2.close();
            outFile3.close();
        }
	}
	else
	{
		// (Just a useful separator for debug cout statements in grammar)
		std::cout << "**********************************************\n";
	}
    
    
	// Close the file stream
	fclose(proccin);
	// Destroy lexer so it reinitializes
	procclex_destroy();
	// Return 1 if syntax error, 0 otherwise
	return static_cast<int>(gSyntaxError);
}

void proccerror(const char* s) // NOLINT
{
	std::cout << s << " on line " << gLineNumber << std::endl;
	gSyntaxError = true;
}

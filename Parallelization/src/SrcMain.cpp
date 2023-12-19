#include "SrcMain.h"
#include <iostream>
#include "FASTAParser.h"
#include "DNATranslator.h"
#include <fstream>
#include "NeedlemanWunschAlgorithm.h"


void ProcessCommandArgs(int argc, const char* argv[])
{
    if(argc == 2) {
        std::string name(argv[1]);
        FASTAParser parser(name);
        std::string sequence = parser.parseFile();

        DNATranslator translator(sequence);
        translator.Translate();
        
        std::ofstream outFile ("amino.txt");
        if (outFile.is_open()) {
            
            outFile << parser.mHeader << "\n";
            
            outFile << "Total amino acids produced: " << translator.mAminoAcidTotal << "\n";
            
            for (int i = 0; i < translator.mAminoAcidCount.size(); i++) {
                outFile << translator.mAminoAcidCount[i].first << ": " << translator.mAminoAcidCount[i].second << "\n";
            }
            outFile.close();
        }
    }
    if (argc == 3) {
        NeedlemanWunschAlgorithm solver(argv[1], argv[2]);
        std::pair<std::string, std::string> solvedSequences = solver.NMAlgorithm();
        solver.outputMatch(solvedSequences);
    }
}

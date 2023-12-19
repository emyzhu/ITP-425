#include "SrcMain.h"
#include "sha1.h"
#include "HashTable.hpp"
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <tbb/parallel_for_each.h>
#include <tbb/parallel_for.h>
#include <tbb/blocked_range.h>

void ProcessCommandArgs(int argc, const char* argv[])
{
    if (strcmp( argv[1], "dictionary") == 0) {
        // 1. Take a list of common passwords
        std::vector<std::string> dict;
        std::string line;
        std::ifstream file (argv[3]);
        
        if (file.is_open()) {
            while (getline (file, line)) {
                dict.emplace_back(line);
            }
            file.close();
        }
        
        // 2. Encrypt them using sha1
        std::vector<std::string> encrypted;
        
        for (int i = 0; i < dict.size(); i++) {
            unsigned char hash[20];
            sha1::Calc(dict[i].c_str(), dict[i].length(), hash);

            std::string hexStr;
            hexStr.resize(40);
            sha1::ToHexString(hash, &hexStr[0]);
            encrypted.emplace_back(hexStr);
        }
       
        // 3. Store in the hash table (key is encrypted string, value is original text)
        
        HashTable<std::string> ht (std::stoi(argv[2]));
        
        for (int i = 0; i < dict.size(); i++) {
            ht.Insert(encrypted[i], dict[i]);
        }
        
        // 4. Then lookup each “unknown” sha1 encrypted password and see if it’s in the dictionary
        std::vector<std::string> pass;
        std::string line2;
        std::ifstream file2 (argv[4]);
        
        std::vector<std::pair<std::string, std::string>> solved;
        if (file2.is_open()) {
            while (getline (file2, line2)) {
                std::string* encryptedPass = ht.Find(line2);
                if(encryptedPass != nullptr) {
                    solved.emplace_back(std::make_pair(line2, *encryptedPass));
                }
            }
            file2.close();
        }
        
        std::ofstream ofile ("solved.txt");
        if (ofile.is_open()) {
            for(int i = 0; i < solved.size(); i++)
            {
                 ofile << solved[i].first << "," << solved[i].second << std::endl;
            }
            ofile.close();
        }
    }
    if (strcmp( argv[1], "phrases") == 0) {
        std::vector<std::string> pass;
        std::string line;
        std::ifstream file (argv[3]);
        
        if (file.is_open()) {
            while (getline (file, line)) {
                pass.emplace_back(line);
            }
            file.close();
        }
        
        int length = 4;
        
        std::vector<std::string> permutations;
        find_permutations(permutations, pass, length, pass.size(), "");
        
        HashTable<int> ht (std::stoi(argv[2]));
        
        std::string line2;
        std::ifstream file2 (argv[4]);
        int lineTracker = 0;
        
        if (file2.is_open()) {
            while (getline (file2, line2)) {
                ht.Insert(line2, lineTracker);
                ++lineTracker;
            }
            file2.close();
        }
        
        
        std::vector<std::pair<std::string, std::string>> solved(lineTracker);
        
        tbb::parallel_for(tbb::blocked_range<size_t>(0, permutations.size()), [&permutations, &ht, &solved](const tbb::blocked_range<size_t>& r) {
            
            std::string tempStr;
            for (size_t i = r.begin(); i != r.end(); ++i) {
                unsigned char hash[20];
                sha1::Calc(permutations[i].c_str(), permutations[i].length(), hash);

                std::string pHashed;
                pHashed.resize(40);
                sha1::ToHexString(hash, &pHashed[0]);
                     
                if(!(ht.Find(pHashed) == nullptr)) {
                    int solvedIndex = *ht.Find(pHashed);
                    std::pair<std::string, std::string> pair = std::make_pair(pHashed, permutations[i]);
                    solved[solvedIndex] = pair;
                }
            }
            
        });
                          
//        tbb::parallel_for_each(permutations.begin(), permutations.end(), [&ht, &solved] (std::string& p) {
//            unsigned char hash[20];
//            sha1::Calc(p.c_str(), p.length(), hash);
//
//            std::string pHashed;
//            pHashed.resize(40);
//            sha1::ToHexString(hash, &pHashed[0]);
//
//            if(!(ht.Find(pHashed) == nullptr)) {
//                int solvedIndex = *ht.Find(pHashed);
//                std::pair<std::string, std::string> pair = std::make_pair(pHashed, p);
//                solved[solvedIndex] = pair;
//            }
//
//        });
        
        solved.shrink_to_fit();
        
        std::ofstream ofile ("solved.txt");
        if (ofile.is_open()) {
            for(int i = 0; i < solved.size(); i++)
            {
                ofile << solved[i].first << "," << solved[i].second << std::endl;
            }
            ofile.close();
        }
        
    }
    
}

void find_permutations(std::vector<std::string>& permutations, std::vector<std::string>& pass, int length, int pass_size, std::string combo) {
    if (length == 1) {

        for (int i = 0; i < pass_size; ++i) {
            permutations.emplace_back(combo + pass[i]);
        }

    }
    else {
        for (int i = 0; i < pass_size; ++i) {
            find_permutations(permutations, pass, length - 1, pass_size, combo + pass[i]);
        }
    }
    
}

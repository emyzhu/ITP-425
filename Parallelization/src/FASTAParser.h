#ifndef FASTAParser_h
#define FASTAParser_h
#endif /* FASTAParser_h */

#pragma once
#include <string>
#include <list>
#include <iostream>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <vector>

class FASTAParser {
    
public:
    std::string mFileName;
    std::string mHeader;
    std::string mSequence;
    
    FASTAParser(std::string& fileName)
    : mFileName(fileName)
    , mHeader("")
    , mSequence("")
    { }
    
    inline const std::string& parseFile() {
        std::string seq;
        std::ifstream::pos_type size;
        std::ifstream file (mFileName, std::ios::in|std::ios::ate);
        if (file.is_open())
        {
            size = file.tellg();
            mSequence.reserve(size);
            file.seekg(0, std::ios::beg);
            
            std::string headerLine;
            std::getline(file, headerLine);
            mHeader = headerLine.substr(1);
            
            std::string line;
            while (std::getline(file, line))
                {
                    mSequence += line;
                }
            
            file.close();
        }
        mSequence.erase(remove_if(mSequence.begin(), mSequence.end(), [] (char c) {
            return !isspace || c == 'N';
        }),mSequence.end());
        return mSequence;
    };
    
};

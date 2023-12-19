
#include "IMDbExtract.h"

#include <iostream>
#include <fstream>
#include <istream>
#include <vector>

std::vector<std::vector<std::string>>  IMDbExtract::parseIMDB()
{
    std::ifstream file (mFileName);
    if (file.is_open()) {
        std::string line;
        while (std::getline(file, line)) {
            const char delim = '\t';
            std::vector<std::string> retVal;
            size_t start = 0;
            size_t delimLoc = line.find_first_of(delim, start);
            while (delimLoc != std::string::npos) {
                retVal.emplace_back(line.substr(start, delimLoc - start));
                start = delimLoc + 1;
                delimLoc = line.find_first_of(delim, start);
            }
            retVal.emplace_back(line.substr(start));
            if (retVal[7] == "\\N") {
                retVal[7] = "0";
            }
            if (retVal[5] == "\\N") {
                retVal[5] = "0";
            }
            if (retVal[1] == "movie") {
                retVal.emplace_back(createURL(retVal[0]));
                mMovies.emplace_back(retVal);
            }
        }
    }
    file.close();
    
    return mMovies;
}


std::string IMDbExtract::createURL(std::string ID) {
    return "https://www.imdb.com/title/" + ID + "/";
}

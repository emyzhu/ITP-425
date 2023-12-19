#pragma once

#include <map>
#include <vector>
#include <string>

class IMDbExtract {
public:
    
    std::string mFileName;
    
    std::vector<std::vector<std::string>> mMovies;
    
    
    IMDbExtract(std::string mFileName)
    :mFileName(mFileName)
    { }
    
    std::vector<std::vector<std::string>>  parseIMDB ();
    std::string createURL (std::string ID);

};

//#pragma once
//
//#include "IMDbExtract.h"
//
//#include <cpprest/http_client.h>
//#include <cpprest/filestream.h>
//#include <cpprest/json.h>
//
//
//#include <iostream>
//
//class JSONFetch {
//public:
//    JSONFetch(std::string path)
//    :mRequest(path)
//    ,mRequestType(GetRequestType())
//    ,mResults()
//    { }
//    
//    int GetRequestType();
//    web::json::value RetrieveResults(std::vector<std::vector<std::string>>& dict);
//
//
//    std::string mRequest;
//    int mRequestType;
//    std::vector<std::vector<std::string>> mResults;
//};
//

#include "SrcMain.h"
#include "IMDbExtract.h"
#include "JSONFetch.h"

#pragma comment(lib, "cpprest110_1_1")

#include <iostream>
#include <cpprest/http_listener.h>

using namespace web::http;                 
using namespace web::http::client;
using namespace web::http::experimental::listener;

#define TRACE(msg)            std::wcout << msg
#define TRACE_ACTION(a, k, v) std::wcout << a << L" (" << k << L", " << v << L")\n"

std::vector<std::vector<std::string>> movies;


int GetRequestType(std::string URL) {
    if (URL.find("/movie/name/") != std::string::npos) {
        return 2;
    }
    else {
        return 0;
    }
}

void handle_get(http_request request)
{
   TRACE(L"\nhandle GET\n");
    
    std::string URL = request.relative_uri().path();
    
    std::string IDOrTitle(URL.substr(URL.find_last_of("/") + 1));
    
    int requestType = GetRequestType(URL);
    
    if (requestType == 2) {
        size_t index = 0;
        while (true) {
             index = IDOrTitle.find("%20", index);
             if (index == std::string::npos) break;
            IDOrTitle.replace(index, 3, " ");
             index += 1;
        }
    }
    
    web::json::value jsonData;
    int count = 0;
    std::vector<std::vector<std::string>> results;

    for (auto const & m : movies) {
        

        if( IDOrTitle == m[requestType]) {
            
//            std::cout << "\n" << m[0] << "\n" << m[7] << "\n" << m[2] << "\n" << m[9] << "\n" << m[5] << "\n" << std::endl;
            
//            count += 1;
//
//            jsonData[U("ID")] = web::json::value::string(utility::conversions::to_string_t (m[0]));
//            jsonData[U("Runtime")] = web::json::value::number(std::stoi(m[7]));
//            jsonData[U("Title")]  = web::json::value::string(utility::conversions::to_string_t(m[2]));
//            jsonData[U("URL")]   = web::json::value::string(utility::conversions::to_string_t(m[9]));
//            jsonData[U("Year")]  = web::json::value::number(std::stoi(m[5]));
//
//            std::cout << jsonData.serialize().c_str() << std::endl;
            
            results.emplace_back(m);
        }

    }
    if(results.size() == 1) {
        jsonData[U("ID")] = web::json::value::string(utility::conversions::to_string_t (results[0][0]));
        jsonData[U("Runtime")] = web::json::value::number(std::stoi(results[0][7]));
        jsonData[U("Title")]  = web::json::value::string(utility::conversions::to_string_t(results[0][2]));
        jsonData[U("URL")]   = web::json::value::string(utility::conversions::to_string_t(results[0][9]));
        jsonData[U("Year")]  = web::json::value::number(std::stoi(results[0][5]));
        
        std::cout << jsonData.serialize().c_str() << std::endl;
    }
    
    if (results.size() > 1) {
        for (int i = 0; i < results.size(); ++i) {
            jsonData[i][U("ID")] = web::json::value::string(utility::conversions::to_string_t (results[i][0]));
            jsonData[i][U("Runtime")] = web::json::value::number(std::stoi(results[i][7]));
            jsonData[i][U("Title")]  = web::json::value::string(utility::conversions::to_string_t(results[i][2]));
            jsonData[i][U("URL")]   = web::json::value::string(utility::conversions::to_string_t(results[i][9]));
            jsonData[i][U("Year")]  = web::json::value::number(std::stoi(results[i][5]));

            std::cout << jsonData.serialize().c_str() << std::endl;
        }
    }
    
    if(!results.empty()) {
        request.reply(status_codes::OK, jsonData);
    }
    else {
        std::string typeString;
        if(requestType == 2) {
            typeString = "by that name";
        }
        else {
            typeString = "with that ID";
        }
        std::string msg = "No movie " + typeString + " found";
        jsonData[U("Error")] = web::json::value::string(utility::conversions::to_string_t(msg));
        request.reply(status_codes::NotFound, jsonData);
    }
    
    std::cout << "\ndone\n" << std::endl;
}

void ProcessCommandArgs(int argc, const char* argv[])
{
	// TODO: Implement
    
    std::string fileName = argv[1];
    IMDbExtract data(fileName);
    movies = data.parseIMDB();
    

    
    //https://mariusbancila.ro/blog/2013/08/19/full-fledged-client-server-example-with-cpprest-sdk-110/
    
    http_listener listener("http://localhost:12345/");
    
    listener.support(methods::GET, handle_get);

    
    try
       {
          listener
             .open()
             .then([&listener](){TRACE(L"\nstarting to listen\n");})
             .wait();

          while (true);
       }
       catch (std::exception const & e)
       {
          std::wcout << e.what() << std::endl;
       }
    
}

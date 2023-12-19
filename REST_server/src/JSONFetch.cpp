//#include "JSONFetch.h"
//
//#include <algorithm>
//
//int JSONFetch::GetRequestType() {
//    if (mRequest.find("/movie/name/") != std::string::npos) {
//        return 2;
//    }
//    else {
//        return 0;
//    }
//}
//
//web::json::value JSONFetch::RetrieveResults(std::vector<std::vector<std::string>>& dict) {
//    
//    std::string IDOrTitle(mRequest.substr(mRequest.find_last_of("/") + 1));
//    
//    if (mRequestType == 2) {
//        size_t index = 0;
//        while (true) {
//             index = IDOrTitle.find("%20", index);
//             if (index == std::string::npos) break;
//            IDOrTitle.replace(index, 3, " ");
//             index += 1;
//        }
//    }
//    
//    
////    std::cout << IDOrTitle << std::endl;
//    
//    web::json::value jsonData;
//    int count;
//
//    for (auto const & m : dict) {
//
//        if( IDOrTitle == m[mRequestType]) {
//
//            
//            count += 1;
//            
//            jsonData[count - 1][U("ID")] = web::json::value::string(utility::conversions::to_string_t (m[0]));
//            jsonData[count - 1][U("Runtime")] = web::json::value::number(std::stoi(m[7]));
//            jsonData[count][U("Title")]  = web::json::value::string(utility::conversions::to_string_t(m[2]));
//            jsonData[count - 1][U("URL")]   = web::json::value::string(utility::conversions::to_string_t(m[9]));
//            jsonData[count - 1][U("Year")]  = web::json::value::number(std::stoi(m[5]));
//            
////            std::cout << movie.serialize().c_str() << std::endl;
//        }
//
//    }
//    
//    return jsonData;
//}
//
//
//
//

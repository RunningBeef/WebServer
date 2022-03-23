//
// Created by yonggeng on 3/23/22.
//

#include "../include/HttpRequest.h"

std::ostream & operator << (std::ostream & count,const HttpRequest & httpRequest){

    std::cout << "method: " <<  httpRequest.method_ << "  " << "url: " << httpRequest.url_
    << "  " << "version: " << httpRequest.version_ << std:: endl;
    for(auto it : httpRequest.httpHeaderMap){
        std::cout << it.first << " :   " << it.second << std::endl;
    }
    std::cout << httpRequest.content_ << std::endl;
}

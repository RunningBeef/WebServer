//
// Created by yonggeng on 3/23/22.
//

#ifndef RUNNINGBEEF_WEBSERVER_HTTPREQUEST_H
#define RUNNINGBEEF_WEBSERVER_HTTPREQUEST_H
#include<iostream>
#include<unordered_map>

class HttpRequest;
std::ostream & operator << (std::ostream & ,const HttpRequest &);/*输出整理好的Http请求报文的各个字段*/

class HttpRequest{
    friend std::ostream & operator << (std::ostream &,const HttpRequest & );
    enum HTTP_VERSION{/*HTTP版本*/
        HTTP1_0 = 0,HTTP1_1,VERSION_NOT_SPORT
    };
    enum HTTP_METHOD{/*支持的HTTP方法*/
        GET = 0,HEAD,POST,PUT,DELETE,OPTIONS,STRACE,CONNECT,METHOD_NOT_SPORT
    };
    enum HTTP_HEADER {/*请求首部字段*/
        HOST = 0, USER_AGENT, ACCEPT, ACCEPT_LANGUAGE, ACCEPT_ENCODING, CONNECTION,
        IF_MODIFIED_SINCE, IF_NONE_MATCH, CACHE_CONTROL
    };
    std::unordered_map<HTTP_HEADER,std::string> httpHeaderMap;/*保存各个首部字段*/

    HttpRequest(HTTP_METHOD method = METHOD_NOT_SPORT,std::string url = "",HTTP_VERSION version = VERSION_NOT_SPORT):
    method_(method),url_(url),version_(version),content_(""){};
    HTTP_METHOD method_;
    HTTP_VERSION version_;
    std::string url_;
    std::string content_;/*报文主体*/
};
#endif //RUNNINGBEEF_WEBSERVER_HTTPREQUEST_H

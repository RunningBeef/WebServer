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
public:
    enum HTTP_VERSION{/*HTTP版本*/
        HTTP1_0 = 0,HTTP1_1,VERSION_NOT_SPORT
    };
    enum HTTP_METHOD{/*支持的HTTP方法*/
        GET = 0,HEAD,POST,PUT,DELETE,OPTIONS,STRACE,CONNECT,METHOD_NOT_SPORT
    };
    enum HTTP_HEADER {/*请求首部字段*/
        Host = 0,
        User_Agent,
        Connection,/*：keep-alive保持长连接*/
        Accept_Encoding,
        Accept_Language,
        Accept,
        Cache_Control,
        Upgrade_Insecure_Requests
    };
    static std::unordered_map<std::string, HTTP_HEADER> header_map;
    struct EnumClassHash {
        template<typename T>
        std::size_t operator()(T t) const {
            return static_cast<std::size_t>(t);
        }
    };

public:
    HttpRequest(HTTP_METHOD method = METHOD_NOT_SPORT,std::string url = "",HTTP_VERSION version = VERSION_NOT_SPORT):
    method_(method),url_(url),version_(version),content_(""){};
    HTTP_METHOD method_;
    std::string url_;
    HTTP_VERSION version_;
    std::string content_;/*报文主体*/

    std::unordered_map<HTTP_HEADER,std::string,EnumClassHash> headerMap_;/*保存各个首部字段*/
};
#endif //RUNNINGBEEF_WEBSERVER_HTTPREQUEST_H

//
// Created by yonggeng on 3/23/22.
//
//
#ifndef RUNNINGBEEF_WEBSERVER_HTTPREQUEST_H
#define RUNNINGBEEF_WEBSERVER_HTTPREQUEST_H
// #pragma once


#include<iostream>
#include<unordered_map>


class HttpRequest;

/*重载<<运算符 输出整理好的Http请求报文的各个字段*/
std::ostream & operator << (std::ostream & ,const HttpRequest &);

class HttpRequest{
    /* 友元 */
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

    /* 首部字段对应的枚举变量*/
    static std::unordered_map<std::string, HTTP_HEADER> static_stringToHeader_map_;
    /* 哈希函数，其实也可以不用额外写*/
    struct EnumClassHash {
        template<typename T>
        std::size_t operator()(T t) const {
            return static_cast<std::size_t>(t);
        }
    };

public:
    void setMethod(HTTP_METHOD method){method_ = method;}
    HTTP_METHOD getMethod(){return method_;}

    void setUrl(std::string url){url_ = url;}
    std::string getUrl(){return url_;}

    void setVersion(HTTP_VERSION version){version_ = version;}
    HTTP_VERSION getVersion(){return version_;}

    void setContent(std::string content){content_ = content_;}
    std::string getContent(){return content_;}

public:
    HttpRequest(HTTP_METHOD method = METHOD_NOT_SPORT,std::string url = "",HTTP_VERSION version = VERSION_NOT_SPORT):
    method_(method),url_(url),version_(version),content_(""){};
public:/*有空改成private*/
    HTTP_METHOD method_;
    std::string url_;
    HTTP_VERSION version_;
    std::string content_;/*报文主体*/
public:
    /*存储首部字段对应的值 */
    std::unordered_map<HTTP_HEADER,std::string,EnumClassHash> headerToStringMap_;
};
#endif

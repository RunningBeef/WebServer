#ifndef HTTTPREQUEST_H
#define HTTPREQUEST_H
#include <string>
#include <unordered_map>
#include <iostream>
#include "ParseHttpRequest.h"
std::ostream & operator << (std::ostream & cout, HttpRequest & httpRequest);
class HttpRequest
{
public:
      friend std::ostream & operator << (std::ostream & cout, HttpRequest & HttpRequest);
      friend ParseHttpRequest;
enum class HttpMethod
      {
            KGet = 0,
            KHead,
            KPost,
            KPut,
            KDelete,
            KPatch,
            KTrace,
            KConnect,
            KMethodNotSupport
      };
      enum class HttpVersion
      {
            KHttp1_0 = 0,
            KHttp1_1,
            KVersionNotSupport
      };
      enum class HttpHeader
      {
            KHost = 0,
            //存放具体站点信息，一个IP地址（虚拟机）可以部署多个站点，根据HOST来区分具体站点
            KUser_Agent,
            //用户使用的什么工具（浏览器）和版本发送请求
            KConnection,
            // HTTP1_1默认开启长连接，keep-alive
            KAccept_Language,
            //浏览器支持的语言，包含权重比例
            KAccept_Encoding,
            //浏览器向客户端提议压缩算法，如gzip,bzip
            KAccept,
            //浏览器希望接收的数据类型，如text/xml,application/json
            KCache_Control,
            //是否可缓存，以及过期时间
            KUpGrade_Insecure_Request,
            //表示用户对加密和认证响应的
            KCookie
      };
      HttpRequest(HttpMethod method = HttpMethod::KMethodNotSupport, std::string url = "/",
                  HttpVersion http_version = HttpVersion::KVersionNotSupport,std::string body = "");
      ~HttpRequest() = default;

      static std::unordered_map<std::string, HttpHeader> string_to_http_header;
private:
      HttpMethod method_;
      std::string url_;
      HttpVersion http_version_;
      std::string body_;
      std::unordered_map<HttpHeader,std::pair<std::string,std::string> > httpHeader_;
};
#endif
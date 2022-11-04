#ifndef HTTTPREQUEST_H
#define HTTPREQUEST_H
#include <string>
#include <unordered_map>
#include <iostream>
#include <vector>
#include "ParseHttpRequest.h"

class ParseHttpRequest;

class HttpRequest
{
      friend std::ostream &operator<<(std::ostream &out, const HttpRequest &HttpRequest);
      friend ParseHttpRequest;

public:
      enum HttpMethod
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
      enum HttpVersion
      {
            KHttp1_0 = 0,
            KHttp1_1,
            KVersionNotSupport
      };
      enum HttpRequestHeader
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
                  HttpVersion http_version = HttpVersion::KVersionNotSupport, std::string body = "");
      ~HttpRequest() = default;

      static std::unordered_map<std::string, HttpRequestHeader> string_to_http_header;
      static std::unordered_map<std::string, HttpMethod> string_to_http_method;
      void setMethod(HttpMethod);
      void setUrl(std::string &);
      void setHttpVersion(HttpVersion);
      void setHttpBody(std::string &);

      HttpMethod getHttpMethod();
      std::string getHttpUrl();
      HttpVersion getHttpVersion();
      std::string getHttpBody();

private:
      HttpMethod method_;
      std::string url_;
      HttpVersion http_version_;
      std::string body_;
      std::unordered_map<HttpRequestHeader, std::pair<std::string, std::string>> http_header_;
      HttpParseState parseResult;
};
#endif
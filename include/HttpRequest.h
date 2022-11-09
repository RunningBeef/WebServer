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
      // 目前支持GET,HEAD,POST
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
      /*
      https://zhuanlan.zhihu.com/p/397151734 http各个版本介绍
      HTTP/1.0如今仍然广泛使用，默认短连接，
      所以有些浏览器会加上Connection: keep-alive表示长连接
      HTTP1.1默认长连接
      后续需要支持管道机制,文件断点续传
      客户端最后一个请求发送Connection: close
      */
      enum HttpVersion
      {
            KHttp1_0 = 0,
            KHttp1_1,
            KVersionNotSupport
      };

      // Http首部字段分为 通用首部，请求首部（请求专用），响应首部（响应专用），实体首部
      //这里只列出部分需要使用的
      enum HttpRequestHeader //只列出少部分
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
            KCookie,
            //实体长度
            KContent_Length,
            //实体类型
            KContent_Type
      };

      const static std::unordered_map<std::string, HttpRequestHeader> KRequestHeaderMap;
      const static std::unordered_map<std::string, HttpMethod> KHttpMethodMap;
      const static std::vector<std::string> request_header_v;
      const static std::vector<std::string> http_version_v;
      HttpRequest();
      HttpRequest(const HttpRequest &) = default;
      HttpRequest &operator=(const HttpRequest &) = default;
      ~HttpRequest() = default;

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
      std::unordered_map<HttpRequestHeader, std::string> http_header_;
      ParseHttpRequest::HttpParseState parseResult;
};
#endif
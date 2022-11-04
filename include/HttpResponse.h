#ifndef HTTPRESPONSE_H
#define HTTPRESPONSE_H
#include "HttpRequest.h"
#include <unordered_map>
#include <iostream>
#include <string>
class HttpResponse
{
      friend std::ostream &operator<<(std::ostream &out, const HttpResponse &httpResponse);

public:
      enum StatueCode
      {
            K200Ok = 0,
            //请求成功
            K301MovedPermanently,
            //永久重定向
            K302Found,
            //临时重定向
            K400BadRequest,
            //客户端请求语法错误,HttpRequest里提取
            K403Forbidden,
            //服务器理解客户端的请求，但是拒绝了该请求，文件权限不够等原因
            K404NotFound,
            //服务器没有找到请求的资源
            K500InternalServerError
            //服务器内部错误，无法完成请求,HttpRequest里提取
      };

      std::vector<std::string> statue_code_string = {
          "200",
          "301",
          "302",
          "400",
          "403",
          "404",
          "500"};
      std::vector<std::string>
          reason_phrase = {"OK", "Moved Permanently", "Found", "Bad Request", "Forbidden", "Not Found", "Internal Server Error"};

      enum HttpResponseHeader
      {

      };
      static std::unordered_map<std::string, HttpResponseHeader> stringToResponseHeaderMap;
      HttpResponse();
      ~HttpResponse() = default;

private:
      HttpRequest::HttpVersion http_version_;
      StatueCode statue_code_;
      std::string reason_phrase_;
      static std::unordered_map<HttpResponseHeader, std::pair<std::string, std::string>> response_header_map_;
      char *body_;
};

#endif

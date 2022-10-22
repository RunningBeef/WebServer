#ifndef HTTTPREQUEST_H
#define HTTPREQUEST_H
#include <string>

class HttpRequest
{
public:
      enum class METHOD
      {
            GET = 0,
            HEAD,
            POST,
            PUT,
            DELETE,
            PATCH,
            TRACE,
            CONNECT,
            METHOD_NOT_SUPPORT
      };
      enum class HttpVersion
      {
            HTTP1_0 = 0,
            HTTP1_1,
            VERSION_NOT_SUPPORT
      };
      enum class HttpHeader
      {
            ACCEPT = 0,
            
      } public:
          private : std::string method_;
      std::string url_;
      HttpVersion http_version;
      std::string body;
};
#endif
#ifndef HTTTPREQUEST_H
#define HTTPREQUEST_H
#include <string>

enum class METHOD
{
      GET = 0,
      HEAD,
      POST,
      PUT,
      DELETE,
      PATCH,
      TRACE,
      CONNECT
};
enum class HttpVersion
{
      HTTP_1_0 = 0,
      HTTP_1_1
};

class HttpRequest
{
public:
private:
      std::string method_;
      std::string url_;
      
};
#endif
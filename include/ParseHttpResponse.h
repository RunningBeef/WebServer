#ifndef PARSEHTTPRESPONSE_H
#define PARSEHTTPRESPONSE_H
#include "HttpResponse.h"
#include "HttpRequest.h"
class HttpRequest;
class HttpResponse;

class ParseHttpResponse{
public:
      ParseHttpResponse(HttpRequest & http_request, HttpResponse & http_response);
      void parse();
      HttpRequest & http_request_;
      HttpResponse & http_response_;
};

#endif
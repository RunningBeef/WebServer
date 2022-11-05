#ifndef PARSEHTTPRESPONSE_H
#define PARSEHTTPRESPONSE_H
#include "HttpResponse.h"
#include "HttpRequest.h"
class HttpRequest;
class HttpResponse;

class ParseHttpResponse{
public:
      ParseHttpResponse(std::shared_ptr<HttpRequest> http_request_ptr, std::shared_ptr<HttpResponse> http_response_ptr);
      void parse();
      std::shared_ptr<HttpRequest> http_request_ptr_;
      std::shared_ptr<HttpResponse> http_response_ptr_;
};

#endif
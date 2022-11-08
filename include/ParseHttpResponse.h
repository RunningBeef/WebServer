#ifndef PARSEHTTPRESPONSE_H
#define PARSEHTTPRESPONSE_H
#include "HttpRequest.h"
#include "HttpResponse.h"
#include "ParseHttpRequest.h"
#include "Timer.h"
#include "Socket.h"
class ParseHttpResponse
{

public:
      ParseHttpResponse(std::shared_ptr<ClientSocket> client_socket_ptr,
               std::shared_ptr<HttpRequest> http_request_ptr,
               std::shared_ptr<HttpResponse> http_response_ptr,
               std::weak_ptr<TimerNode> timer_ptr
               ) {}
      void appendStateLine();
      void appendHeader();
      void appendBody();
private:
      std::shared_ptr<ClientSocket> client_socket_ptr_;
      std::shared_ptr<HttpRequest> http_request_ptr_;
      std::shared_ptr<HttpResponse> http_response_ptr_;
      std::weak_ptr<TimerNode> timer_ptr_;
};
#endif
#ifndef HTTPDATA_H
#define HTTPDATA_H
#include "HttpRequest.h"
#include "HttpResponse.h"
#include "ParseHttpRequest.h"
#include "ParseHttpResponse.h"
#include "Timer.h"
#include "Socket.h"
class HttpData
{

public:
      HttpData(std::shared_ptr<ClientSocket> client_socket_ptr,
               std::shared_ptr<HttpRequest> http_request_ptr,
               std::shared_ptr<HttpResponse> http_response_ptr,
               std::shared_ptr<ParseHttpRequest> parse_http_request_ptr,
               std::shared_ptr<ParseHttpResponse> parse_http_response_ptr,
               std::weak_ptr<TimerNode> timer_ptr,
               int client_fd) {}
      std::shared_ptr<ClientSocket> client_socket_ptr_;
      std::shared_ptr<HttpRequest> http_request_ptr_;
      std::shared_ptr<HttpResponse> http_response_ptr_;
      std::shared_ptr<ParseHttpRequest> parse_http_request_ptr_;
      std::shared_ptr<ParseHttpResponse> parse_http_response_ptr_;
      std::weak_ptr<TimerNode> timer_ptr_;
      int client_fd_;
};
#endif
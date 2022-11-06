#include "../include/HttpData.h"

HttpData::HttpData(std::shared_ptr<ClientSocket> client_socket_ptr,
                   std::shared_ptr<HttpRequest> http_request_ptr,
                   std::shared_ptr<HttpResponse> http_response_ptr,
                   std::shared_ptr<ParseHttpRequest> parse_http_request_ptr,
                   std::shared_ptr<ParseHttpResponse> parse_http_response_ptr,
                   std::weak_ptr<TimerNode> timer_ptr,
                   int client_fd)
    : client_socket_ptr_(client_socket_ptr), http_request_ptr_(http_request_ptr),
      http_response_ptr_(http_response_ptr), parse_http_request_ptr_(parse_http_request_ptr),
      parse_http_response_ptr_(parse_http_response_ptr_), timer_ptr_(timer_ptr), client_fd_(client_fd)
{
}
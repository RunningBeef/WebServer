#ifndef HTTPDATA_H
#define HTTPDATA_H
#include "HttpRequest.h"
#include "ParseHttpRequest.h"
#include "HttpResponse.h"
#include "ParseHttpResponse.h"
#include "../timer/Timer.h"
#include "../server/Socket.h"
//引用不会执行析构函数
class HttpData
{
      HttpData(std::shared_ptr<ClientSocket> &ClientSocket);
private:
      std::shared_ptr<HttpRequest> http_request_ptr_;
      std::shared_ptr<HttpResponse> http_response_ptr_;
      std::shared_ptr<ParseHttpRequest> parse_request_ptr_;
      std::shared_ptr<ParseHttpResponse> parse_response_ptr_;
      std::shared_ptr<ClientSocket> & client_socket_ptr;
};
#endif
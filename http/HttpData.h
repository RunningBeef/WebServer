#ifndef HTTPDATA_H
#define HTTPDATA_H
#include "HttpRequest.h"
#include "ParseHttpRequest.h"
#include "HttpResponse.h"
#include "ParseHttpResponse.h"
#include "../timer/Timer.h"
#include "../server/Socket.h"
//引用不会执行析构函数
class HttpData : public NonCopyable
{
      HttpData(std::shared_ptr<ClientSocket> &,std::shared_ptr<TimerNode> &);
private:
      std::shared_ptr<HttpRequest> m_http_request_ptr_;
      std::shared_ptr<HttpResponse> m_http_response_ptr_;
      std::shared_ptr<ParseHttpRequest> m_parse_request_ptr_;
      std::shared_ptr<ParseHttpResponse> m_parse_response_ptr_;
      std::shared_ptr<ClientSocket> m_client_socket_ptr_;
      std::weak_ptr<TimerNode> m_timer_node_ptr_;
};
#endif
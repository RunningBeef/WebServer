#include "HttpData.h"

HttpData::HttpData(std::shared_ptr<ClientSocket> & client_socket_ptr,std::shared_ptr<TimerNode> & timer_node_ptr)
:m_client_socket_ptr_(client_socket_ptr),m_timer_node_ptr_(timer_node_ptr)
{
      m_http_request_ptr_ = std::make_shared<HttpRequest>();
      m_http_response_ptr_ = std::make_shared<HttpResponse>();
      m_parse_request_ptr_ = std::make_shared<ParseHttpRequest>(m_http_request_ptr_,m_client_socket_ptr_);
      m_parse_response_ptr_ = std::make_shared<ParseHttpResponse>(m_http_response_ptr_,m_client_socket_ptr_);
}
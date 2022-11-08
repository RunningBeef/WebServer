#include "../include/ParseHttpResponse.h"

ParseHttpResponse::ParseHttpResponse(std::shared_ptr<ClientSocket> client_socket_ptr,
                                     std::shared_ptr<HttpRequest> http_request_ptr,
                                     std::shared_ptr<HttpResponse> http_response_ptr,
                                     std::weak_ptr<TimerNode> timer_ptr)
    : client_socket_ptr_(client_socket_ptr), http_request_ptr_(http_request_ptr),
      http_response_ptr_(http_response_ptr), timer_ptr_(timer_ptr)
{
    appendStateLine();
    appendHeader();
    appendBody();
}

/*
    支持GET/POST，
    查找输出静态文件
    支持cookie
    支持长连接

    支持登录注册
 */

void ParseHttpResponse:: appendStateLine()
{
    
}
void ParseHttpResponse:: appendHeader()
{
    
}
void ParseHttpResponse::appendBody()
{

}

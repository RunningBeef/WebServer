#include "../include/ParseHttpResponse.h"

ParseHttpResponse::ParseHttpResponse(std::shared_ptr<ClientSocket> client_ptr,
                                     std::shared_ptr<HttpRequest> http_request_ptr,
                                     std::shared_ptr<HttpResponse> http_response_ptr,
                                     std::weak_ptr<TimerNode> timer_ptr)
    : client_ptr_(client_ptr), http_request_ptr_(http_request_ptr),
      http_response_ptr_(http_response_ptr), timer_ptr_(timer_ptr)
{
}

void ParseHttpResponse::postLog(ParseHttpResponse *)
{
}

void ParseHttpResponse::postRegister(ParseHttpResponse *)
{
}
/*
考虑后期接口比较多，为了快速确定需要调用的业务接口，
可以直接来一个URL(string)转函数接口的的调用
这里注意什么和构造类型要一致
*/
const std::unordered_map<std::string, std::function<void(ParseHttpResponse *)>>
    ParseHttpResponse ::KPostUrlFuncMap =
        {
            {"/log", postLog},
            {"/register", postRegister}
        };

/*
    支持GET/POST，
    查找输出静态文件
    支持cookie
    支持长连接

    支持登录注册
 */

// 不合适的http版本已经在ParseHttpRequest处理了
void ParseHttpResponse::appendStateLine()
{
    if(http_request_ptr_->getHttpMethod() == HttpRequest::HttpMethod::KGet)
    {

    }
    else
    {

    }
}
void ParseHttpResponse::appendHeader()
{
}
void ParseHttpResponse::appendBody()
{
}

void ParseHttpResponse::writeResponse()
{
}
void ParseHttpResponse::parse()
{
    if (http_request_ptr_->getParseResult() != ParseHttpRequest::HttpParseState::KGetRequest)
    {
        /* 断开连接 */
        return;
    }
    appendStateLine();
    appendHeader();
    appendBody();
    writeResponse();
}
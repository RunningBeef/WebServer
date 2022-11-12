#include "ParseHttpResponse.h"
/*
    支持GET/POST，
    查找输出静态文件
    支持cookie
    支持长连接
    支持登录注册
 */

const std::string KBasePath = "";
/*
考虑后期接口比较多，为了快速确定需要调用的业务接口，
可以直接来一个URL(string)转函数接口的的调用
这里注意什么和构造类型要一致
*/
const std::unordered_map<std::string, std::function<void(ParseHttpResponse *)>>
    ParseHttpResponse ::KPostUrlFuncMap =
        {
            {"/log", postLog},
            {"/register", postRegister}};

const std::unordered_map<std::string, std::string>
    ParseHttpResponse::KErrorPagePathMap = {
        {"Not Found", "/resources/404.html"},
        {"Method Not Allowed", "/resources/405.html"}};

ParseHttpResponse::
    ParseHttpResponse(std::shared_ptr<ClientSocket> client_ptr,
                      std::shared_ptr<HttpRequest> http_request_ptr,
                      std::shared_ptr<HttpResponse> http_response_ptr,
                      std::weak_ptr<TimerNode> timer_ptr)
    : client_ptr_(client_ptr),
      http_request_ptr_(http_request_ptr),
      http_response_ptr_(http_response_ptr),
      timer_ptr_(timer_ptr)
{
}

void ParseHttpResponse::postLog(ParseHttpResponse *)
{
}

void ParseHttpResponse::postRegister(ParseHttpResponse *)
{
}

void ParseHttpResponse::checkFile()
{
    std::string file_path = KBasePath + http_request_ptr_->getHttpUrl();
    struct stat buf;
    if (stat(file_path.c_str(), &buf))
    {
        error_path_ = "Not Found";
    }
    else
    {
        http_response_ptr_->statue_code_ = 200;
        http_response_ptr_->http_version_ = HttpRequest::HttpVersion::KHttp1_1;
    }
}

void ParseHttpResponse::checkInterface()
{
    if (!KPostUrlFuncMap.count(http_request_ptr_->getHttpUrl()))
    {
        error_path_ = "Not Found";
    }
    else
    {
        http_response_ptr_->statue_code_ = 200;
        http_response_ptr_->http_version_ = HttpRequest::HttpVersion::KHttp1_1;
    }
}

// 不合适的http版本已经在ParseHttpRequest处理了
void ParseHttpResponse::appendStateLine()
{
    HttpRequest::HttpMethod method = http_request_ptr_->getHttpMethod();
    switch (method)
    {
    case HttpRequest::HttpMethod::KGet:
        checkFile();
    case HttpRequest::HttpMethod::KPost:
        checkInterface();
    default:
        error_path_ = "Method Not Allowed";
    }
}
void ParseHttpResponse::appendHeader()
{
    if()
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
        /* 断开连接 关闭资源*/
        return;
    }
    appendtSateLine();
    appendHeader();
    appendBody();
    writeResponse();
}
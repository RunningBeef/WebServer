#include "ParseHttpResponse.h"

const std::string KBasePath = "/home/lighthouse/WebServer/resources";
/*
考虑后期接口比较多，为了快速确定需要调用的业务接口，
可以直接来一个URL(string)转函数接口的的调用
这里注意什么和构造类型要一致
*/
// const std::unordered_map<std::string, std::function<void(ParseHttpResponse *)>>
//     ParseHttpResponse ::KPostUrlFuncMap =
//         {
//             {"/log", postLog},
//             {"/register", postRegister}};

const std::unordered_map<std::string, std::string>
    ParseHttpResponse::KErrorPagePathMap =
        {
            {"Not Found", "/resources/404.html"},
            {"Method Not Allowed", "/resources/405.html"}};
const std::unordered_map<std::string, std::string>
    ParseHttpResponse::KFileTypeMap =
        {
            {".html", "text/html"},
            {".xml", "text/xml"},
            {".xhtml", "application/xhtml+xml"},
            {".txt", "text/plain"},
            {".rtf", "application/rtf"},
            {".pdf", "application/pdf"},
            {".word", "application/nsword"},
            {".png", "image/png"},
            {".gif", "image/gif"},
            {".jpg", "image/jpeg"},
            {".jpeg", "image/jpeg"},
            {".au", "audio/basic"},
            {".mpeg", "video/mpeg"},
            {".mpg", "video/mpeg"},
            {".avi", "video/x-msvideo"},
            {".gz", "application/x-gzip"},
            {".tar", "application/x-tar"},
            {".css", "text/css "},
            {".js", "text/javascript "},
};
const std::string ParseHttpResponse::KLineEnd = "\r\n";
const std::string ParseHttpResponse::KKeyValueSeparate = ": ";
const std::string ParseHttpResponse::KSpace = " ";

ParseHttpResponse::
    ParseHttpResponse(std::shared_ptr<HttpResponse> &http_response_ptr, std::shared_ptr<ClientSocket> &client_ptr)
    : m_client_ptr_(client_ptr),
      m_http_response_ptr_(http_response_ptr)
{
}

bool ParseHttpResponse::checkFile(std::string &url)
{
    std::string absolute_address = KBasePath + url;
    struct stat buf;
    if (stat(absolute_address.c_str(), &buf))
    {
        return false;
    }
    return true;
}

std::string ParseHttpResponse::getFileType(std::string &url)
{
    int id = url.find_last_of('/');
    std::string suffix = url.substr(id + 1, url.size() - id - 1);
    if (KFileTypeMap.count(suffix))
    {
        return KFileTypeMap.find(suffix)->second;
    }
    else
        return "";
}

// 不合适的http版本已经在ParseHttpRequest处理了
void ParseHttpResponse::appendStateLine(std::shared_ptr<HttpResponse> &http_response_ptr_)
{
    http_response_ptr_->m_http_response_ += "HTTP/1.1" + KSpace + std::to_string(http_response_ptr_->m_statue_code_) + KSpace + HttpResponse::KStateCodeMap.find(http_response_ptr_->m_statue_code_)->second + KLineEnd;
}

void ParseHttpResponse::appendHeader(std::shared_ptr<HttpResponse> &http_response_ptr_)
{
    for (auto it : http_response_ptr_->m_response_header_map_)
    {
        http_response_ptr_->m_http_response_ +=
            it.first + KKeyValueSeparate + it.second + KLineEnd;
    }
    http_response_ptr_->m_http_response_ += KLineEnd;
}

void ParseHttpResponse::appendBody(std::shared_ptr<HttpResponse> &http_response_ptr_)
{
    http_response_ptr_->m_http_response_ += http_response_ptr_->m_body_;
}

//后期可以扩展用其他方法写数据
void ParseHttpResponse::writeResponse(std::shared_ptr<HttpResponse> &http_response_ptr_, std::shared_ptr<ClientSocket> &client_socket_ptr_)
{
    ::send(client_socket_ptr_->getClientSocket(), http_response_ptr_->m_http_response_.c_str(), http_response_ptr_->m_http_response_.size(), 0);
}

void ParseHttpResponse::parse(std::shared_ptr<HttpResponse> &http_response_ptr_, std::shared_ptr<ClientSocket> &client_socket_ptr_)
{
    appendStateLine(http_response_ptr_);
    appendHeader(http_response_ptr_);
    appendBody(http_response_ptr_);
    writeResponse(http_response_ptr_, client_socket_ptr_);
}
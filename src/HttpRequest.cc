#include "../include/HttpRequest.h"
std::unordered_map<std::string, HttpRequest::HttpRequestHeader> HttpRequest::string_to_http_header = {
    {"Accept", HttpRequestHeader::KAccept},
    {"Accept-Language", HttpRequestHeader::KAccept_Language},
    {"Accept-Encoding", HttpRequestHeader::KAccept_Encoding},
    {"Cache-Control", HttpRequestHeader::KCache_Control},
    {"Cookie", HttpRequestHeader::KCookie},
    {"Host", HttpRequestHeader::KHost},
    {"Upgrade-Insecure-Request", HttpRequestHeader::KUpGrade_Insecure_Request},
    {"User-Agent", HttpRequestHeader::KUser_Agent}};
std::unordered_map<std::string, HttpRequest::HttpMethod> HttpRequest::string_to_http_method = {
    {"GET", HttpMethod::KGet},
    {"HEAD", HttpMethod::KHead},
    {"POST", HttpMethod::KPost},
    {"PUT", HttpMethod::KPut},
    {"DELETE", HttpMethod::KDelete},
    {"PATCH", HttpMethod::KPatch},
    {"TRACE", HttpMethod::KTrace},
    {"CONNECT", HttpMethod::KConnect}};

void HttpRequest::setMethod(HttpMethod method)
{
    this->method_ = method;
}
void HttpRequest::setUrl(std::string &url)
{
    this->url_ = url;
}
void HttpRequest::setHttpVersion(HttpVersion http_version)
{
    this->http_version_ = http_version;
}

void HttpRequest::setHttpBody(std::string &http_body)
{
    this->body_ = http_body;
}

HttpRequest::HttpMethod HttpRequest::getHttpMethod()
{
    return this->method_;
}
std::string HttpRequest::getHttpUrl()
{
    return this->url_;
}
HttpRequest::HttpVersion HttpRequest::getHttpVersion()
{
    return this->http_version_;
}
std::string HttpRequest::getHttpBody()
{
    return this->body_;
}

HttpRequest::
    HttpRequest(HttpMethod method = HttpMethod::KMethodNotSupport, std::string url = "/",
                HttpVersion http_version = HttpVersion::KVersionNotSupport, std::string body = "")
    : method_(method), url_(url), http_version_(http_version), body_(body),parseResult(HttpParseState::KInternalError)
{
}

std::ostream &operator<<(std::ostream &out,const HttpRequest &httpRequest)
{
    // 这里如果把HttpVersion枚举定义成enum HttpVersion class{}
    // 这里的输出HttpVersion:就会产生no operator << match these operands 错误
    // 但是HttpMethod 却不会报错，很疑惑？
    out << "method: " << httpRequest.method_ << "\turl: " << httpRequest.url_
         << "\tHttpVersion: " << httpRequest.http_version_ << std::endl;
    for (auto it : httpRequest.http_header_)
    {
        out << it.second.first << ": " << it.second.second << std::endl;
    }
    out << httpRequest.body_ << std::endl;
    return out;
}
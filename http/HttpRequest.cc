#include "HttpRequest.h"
const std::unordered_map<std::string, HttpRequest::HttpRequestHeader> HttpRequest::KRequestHeaderMap = {
    {"Accept", HttpRequestHeader::KAccept},
    {"Accept-Language", HttpRequestHeader::KAccept_Language},
    {"Accept-Encoding", HttpRequestHeader::KAccept_Encoding},
    {"Cache-Control", HttpRequestHeader::KCache_Control},
    {"Cookie", HttpRequestHeader::KCookie},
    {"Host", HttpRequestHeader::KHost},
    {"Upgrade-Insecure-Request", HttpRequestHeader::KUpGrade_Insecure_Request},
    {"User-Agent", HttpRequestHeader::KUser_Agent},
    {"Content-Length", HttpRequestHeader::KContent_Length},
    {"Content-Type", HttpRequestHeader::KContent_Type}};
const std::unordered_map<std::string, HttpRequest::HttpMethod> HttpRequest::KHttpMethodMap = {
    {"GET", HttpMethod::KGet},
    {"HEAD", HttpMethod::KHead},
    {"POST", HttpMethod::KPost},
    // {"PUT", HttpMethod::KPut},
    // {"DELETE", HttpMethod::KDelete},
    // {"PATCH", HttpMethod::KPatch},
    // {"TRACE", HttpMethod::KTrace},
    // {"CONNECT", HttpMethod::KConnect}
};

const std::vector<std::string> http_version_v = {
    "HTTP/1.0",
    "HTTP/1.1",
    "RequestNotSupport"};
const std::vector<std::string> request_header_v = {
    "Accept",
    "Accept-Language",
    "Accept-Encoding",
    "Cache-Control",
    "Cookie",
    "Host",
    "Upgrade-Insecure-Request",
    "User-Agent"};

void HttpRequest::setMethod(HttpMethod method)
{
    this->m_method_ = method;
}
void HttpRequest::setUrl(std::string &url)
{
    this->m_url_ = url;
}
void HttpRequest::setHttpVersion(HttpVersion http_version)
{
    this->m_http_version_ = http_version;
}

void HttpRequest::setHttpBody(std::string &http_body)
{
    this->m_body_ = http_body;
}

HttpRequest::HttpMethod HttpRequest::getHttpMethod()
{
    return this->m_method_;
}
std::string HttpRequest::getHttpUrl()
{
    return this->m_url_;
}
HttpRequest::HttpVersion HttpRequest::getHttpVersion()
{
    return this->m_http_version_;
}
std::string HttpRequest::getHttpBody()
{
    return this->m_body_;
}

HttpRequest::
    HttpRequest()
    : m_method_(HttpMethod::KMethodNotSupport), m_http_version_(HttpVersion::KVersionNotSupport), m_parseResult(ParseHttpRequest::HttpParseState::KParseLine)
{
}
ParseHttpRequest::HttpParseState HttpRequest:: getParseResult()
{
    return m_parseResult;
}

std::ostream &operator<<(std::ostream &out, const HttpRequest &httpRequest)
{
    // 这里如果把HttpVersion枚举定义成enum HttpVersion class{}
    // 这里的输出HttpVersion:就会产生no operator << match these operands 错误
    // 但是HttpMethod 却不会报错，很疑惑？
    out << "method: " << httpRequest.m_method_ << "\turl: " << httpRequest.m_url_
        << "\tHttpVersion: " << http_version_v[httpRequest.m_http_version_] << std::endl;
    for (auto it : httpRequest.m_http_header_)
    {
        out << request_header_v[it.first] << ": " << it.second << std::endl;
    }
    out << httpRequest.m_body_ << std::endl;
    return out;
}
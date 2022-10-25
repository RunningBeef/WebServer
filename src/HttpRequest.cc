#include "../include/HttpRequest.h"

std::unordered_map<std::string, HttpHeader> HttpRequest::string_to_http_header = {
    {"Accept", HttpHeader::KAccept},
    {"Accept-Language", HttpHeader::KAccept_Language},
    {"Accept-Encoding", HttpHeader::KAccept_Encoding},
    {"Cache-Control", HttpHeader::KCache_Control},
    {"Cookie", HttpHeader::KCookie},
    {"Host", HttpHeader::KHost},
    {"Upgrade-Insecure-Request", HttpHeader::KUpGrade_Insecure_Request},
    {"User-Agent", HttpHeader::KUser_Agent}};
std::unordered_map<std::string, HttpMethod> HttpRequest::string_to_http_method = {
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

HttpMethod HttpRequest::getHttpMethod()
{
    return this->method_;
}
std::string HttpRequest::getHttpUrl()
{
    return this->url_;
}
HttpVersion HttpRequest::getHttpVersion()
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
    : method_(method), url_(url), http_version_(http_version), body_(body)
{
}

std::ostream &operator<<(std::ostream &cout, HttpRequest &httpRequest)
{
    cout << "method: " << httpRequest.method_ << "\turl: " << httpRequest.url_
         << "\tHttpVersion: " << httpRequest.http_version_ << std::endl;
    for (auto it : httpRequest.http_header_)
    {
        cout << it.second.first << ": " << it.second.second << std::endl;
    }
    cout << httpRequest.body_ << std::endl;
    return cout;
}
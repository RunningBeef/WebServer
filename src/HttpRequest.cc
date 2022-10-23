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
std::unordered_map<std::string,HttpMethod> HttpRequest::string_to_http_method = {
    {"GET",HttpMethod::KGet},
    {"HEAD",HttpMethod::KHead},
    {"POST",HttpMethod::KPost},
    {"PUT",HttpMethod::KPut},
    {"DELETE",HttpMethod::KDelete},
    {"PATCH",HttpMethod::KPatch},
    {"TRACE",HttpMethod::KTrace},
    {"CONNECT",HttpMethod::KConnect}
};


HttpRequest::
HttpRequest(HttpMethod method = HttpMethod::KMethodNotSupport, std::string url = "/",
            HttpVersion http_version = HttpVersion::KVersionNotSupport, std::string body = "")
            :method_(method),url_(url),http_version_(http_version),body_(body)
{}

std::ostream & operator << (std::ostream & cout, HttpRequest & httpRequest)
{
    cout << "method: " << httpRequest.method_ << "\turl: " << httpRequest.url_
    << "\tHttpVersion: " << httpRequest.http_version_ << std::endl;
    for(auto it : httpRequest.httpHeader_)
    {
        cout << it.second.first << ": " << it.second.second << std::endl;
    }
    cout << httpRequest.body_ << std::endl;
    return cout;
}
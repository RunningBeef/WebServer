#include "../include/HttpRequest.h"

std::unordered_map<std::string, HttpRequest::HttpHeader> HttpRequest::string_to_http_header = {
    {"Accept", HttpRequest::HttpHeader::KAccept},
    {"Accept-Language", HttpRequest::HttpHeader::KAccept_Language},
    {"Accept-Encoding", HttpRequest::HttpHeader::KAccept_Encoding},
    {"Cache-Control", HttpRequest::HttpHeader::KCache_Control},
    {"Cookie", HttpRequest::HttpHeader::KCookie},
    {"Host", HttpRequest::HttpHeader::KHost},
    {"Upgrade-Insecure-Request", HttpRequest::HttpHeader::KUpGrade_Insecure_Request},
    {"User-Agent", HttpRequest::HttpHeader::KUser_Agent}};

HttpRequest::
HttpRequest(HttpMethod method = HttpMethod::KMethodNotSupport, std::string url = "/",
            HttpVersion http_version = HttpVersion::KVersionNotSupport, std::string body = "")
            :method_(method),url_(url),http_version_(http_version),body_(body)
{

}

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
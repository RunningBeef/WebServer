#ifndef HTTPRESPONSE_H
#define HTTPRESPONSE_H
#include "HttpRequest.h"
#include "ParseHttpResponse.h"
#include <unordered_map>
#include <unordered_set>
#include <iostream>
#include <string>

class HttpResponse
{
      friend std::ostream &operator<<(std::ostream &out, const HttpResponse &httpResponse);
      friend ParseHttpResponse;
public:
      const static std::unordered_map<int, std::string> KStateCodeMap;

      enum class HttpResponseHeader
      {
            KConnection = 0,
            KKeep_alive,
            KContent_Length,
            KContent_type,
            KContent_Range,
            KSet_Cookie
      };
      HttpResponse();
      ~HttpResponse() = default;
private:
      HttpRequest::HttpVersion m_http_version_;
      int m_statue_code_;
      std::unordered_map<std::string, std::string> m_response_header_map_;
      std::string m_body_;
      std::string m_http_response_;
};

#endif

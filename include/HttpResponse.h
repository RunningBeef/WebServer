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


      enum HttpResponseHeader
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
      const static std::unordered_map<int, std::string> KStateCodeMap;
      const static std::unordered_map<std::string, std::string> KFileTypeMap;
private:
      HttpRequest::HttpVersion http_version_;
      int statue_code_;
      int reason_phrase_id_;
      std::unordered_map<std::string, std::string> response_header_map_;
};

#endif

#include "../include/HttpResponse.h"
std::unordered_map<std::string, HttpResponse::HttpResponseHeader> HttpResponse::stringToResponseHeaderMap = {

};

HttpResponse::HttpResponse() : body_(nullptr)
{
}

std::ostream &operator<<(std::ostream &out, const HttpResponse &httpResponse)
{
      out << "HttpVersion: " << httpResponse.http_version_
          << "\tStatue_code: " << httpResponse.statue_code_
          << "\tReason_phrase: " << httpResponse.reason_phrase_
          << std::endl;
      for (auto it : httpResponse.response_header_map_)
      {
            out << it.second.first << ": " << it.second.second << std::endl;
      }
      out << std::endl;
      if(httpResponse.body_)
      {
            out << "Get HttpResponse Body" <<std::endl;
      }
}
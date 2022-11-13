#include "HttpResponse.h"

const std::unordered_map<int, std::string>
    HttpResponse::KStateCodeMap =
        {
            {200, "OK"},
            {301, "Moved Permanently"},
            {302, "Found"},
            {400, "Bad Request"},
            {403, "Forbidden"},
            {404, "Not Found"},
            {500, "Internal Server Error"}};




std::ostream &operator<<(std::ostream &out, const HttpResponse &httpResponse)
{
      out << "HttpVersion: " << httpResponse.m_http_version_
          << "\tStatue_code: " << httpResponse.m_statue_code_
          << "\tReason_phrase: " << HttpResponse::KStateCodeMap.find(httpResponse.m_statue_code_)->first
          << std::endl;
      for (auto it : httpResponse.m_response_header_map_)
      {
            out << it.first << ": " << it.second << std::endl;
      }
      out << std::endl;
}
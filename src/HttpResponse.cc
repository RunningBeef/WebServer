#include "../include/HttpResponse.h"

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
const std::unordered_map<std::string, std::string>
    HttpResponse::KFileTypeMap =
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



std::ostream &operator<<(std::ostream &out, const HttpResponse &httpResponse)
{
      out << "HttpVersion: " << httpResponse.http_version_
          << "\tStatue_code: " << httpResponse.statue_code_
          << "\tReason_phrase: " << HttpResponse::KStateCodeMap.find(httpResponse.reason_phrase_id_)->first
          << std::endl;
      for (auto it : httpResponse.response_header_map_)
      {
            out << it.first << ": " << it.second << std::endl;
      }
      out << std::endl;
}
#ifndef PARSEHTTPREQUEST_H
#define PARSEHTTPREQUEST_H
#define CR '\r'
#define LF '\n'
#define END '\0'

#include "HttpRequest.h"
#include "ParseString.h"
#include <sstream>
#include <memory>

enum class HttpParseState
{
      KParseLine = 0,
      KParseHeader,
      KParseBody,
      KRequestOk,
      KRequestBad,
      KInternalError
};
enum class LineStatus
{
      KLineMore = 0,
      KLineOk,
      KLineBad,
};
class ParseHttpRequest
{
public:
      ParseHttpRequest(std::shared_ptr<HttpRequest> http_request_ptr, int &end, char * buffer);
      LineStatus parseOneLine();
      void parseRequestLine(HttpParseState &parse_state);
      void parseRequestHeader(HttpParseState &parse_state);
      void parseRequestBody(HttpParseState &parse_state);
      void parseRequest(HttpParseState &parse_state);

private:
      std::shared_ptr<HttpRequest> http_request_ptr_;
      int checked_;
      int uncheck_;
      int &end_; //需要外面提供
      char *buffer_;
};
#endif
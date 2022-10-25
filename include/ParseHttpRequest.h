#ifndef PARSEHTTPREQUEST_H
#define PARSEHTTPREQUEST_H
#define CR '\r'
#define LF '\n'
#define END '\0'

#include "HttpRequest.h"
#include "ParseString.h"
#include <sstream>
class ParseHttpRequest
{
public:
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
      LineStatus parseOneLine();
      void parseRequestLine(HttpParseState &parse_state);
      void parseRequestHeader(HttpParseState &parse_state);
      void parseRequestBody(HttpParseState &parse_state);
      void parseHttpRequest(HttpParseState &parse_state);
private:
      HttpRequest & http_request;
      int checked;
      int uncheck;
      int end;//需要对外提供
      char * buffer;
};
#endif
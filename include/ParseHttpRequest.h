#ifndef PARSEHTTPREQUEST_H
#define PARSEHTTPREQUEST_H
#define CR '\r'
#define LF '\n'
#define END '\0'

#include "HttpRequest.h"
class ParseHttpRequest
{
public:
      enum class HttpParseState
      {
            KRequestLine = 0,
            KRequestHeader,
            KRequestBody,
            KRequestOk,
            KRequestBad
      };
      enum class LineStatus
      {
            KLineMore = 0,
            KLineOk,
            KLineBad,
      };
      LineStatus parseOneLine(char *buffer, int &uncheck, int &end);
      HttpParseState parseRequestLine(char *buffer, int &checked, int &uncheck, HttpRequest & httpRequest);
      HttpParseState parseRequestHeader(char *buffer, int &checked, int &uncheck, HttpRequest & httpRequest);
      HttpParseState parseRequestBody(char &buffer, int &checked, int &uncheck, HttpRequest & httpRequest);
      HttpParseState parseHttpRequest(char *buffer, int &checked, int &uncheck, HttpRequest & httpRequest);
};
#endif
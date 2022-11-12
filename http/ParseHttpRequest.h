#ifndef PARSEHTTPREQUEST_H
#define PARSEHTTPREQUEST_H
#define CR '\r'
#define LF '\n'
#define END '\0'
#define BUFFERSIZE 4096
#include "HttpRequest.h"
#include "ParseString.h"
#include <sstream>
#include <memory>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <Socket.h>
class ParseHttpRequest
{
public:
      enum class HttpParseState
      {
            KParseLine = 0,
            KParseHeader,
            KParseBody,
            KGetRequest,
            KBadRequest,
            KInternalError
      };
      enum class LineStatus
      {
            KLineMore = 0,
            KLineOk,
            KLineBad,
      };
      ParseHttpRequest(std::shared_ptr<HttpRequest> http_request_ptr, int &end, char *buffer);
      LineStatus parseOneLine();
      void parseRequestLine();
      void parseRequestHeader();
      void parseRequestBody();
      void parseRequest();
      bool parse();
      void readRequest();

private:
      std::shared_ptr<HttpRequest> http_request_ptr_;
      std::shared_ptr<ClientSocket> client_ptr_;
      int checked_;
      int uncheck_;
      int end_;
      int body_length_;
      char *buffer_;
};
#endif
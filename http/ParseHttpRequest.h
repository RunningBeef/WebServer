#ifndef PARSEHTTPREQUEST_H
#define PARSEHTTPREQUEST_H
#define CR '\r'
#define LF '\n'
#define END '\0'
#define BUFFERSIZE 4096
#include "HttpRequest.h"
#include "ParseString.h"
#include "../locker/NonCopyable.h"
#include <sstream>
#include <memory>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <Socket.h>

class ParseHttpRequest : public NonCopyable
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
      ParseHttpRequest(std::shared_ptr<HttpRequest> http_request_ptr, std::shared_ptr<ClientSocket> client_socket_ptr);
      ~ParseHttpRequest();
      LineStatus parseOneLine();
      void parseRequestLine();
      void parseRequestHeader();
      void parseRequestBody();
      void parseRequest();
      bool parse();
      void readRequest();
      void init();

private:
      std::shared_ptr<HttpRequest> & http_request_ptr_;
      std::shared_ptr<ClientSocket>  & client_socket_ptr_;
      int checked_;
      int uncheck_;
      int end_;
      int body_length_;
      char *buffer_;
};
#endif
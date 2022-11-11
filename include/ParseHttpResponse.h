#ifndef PARSEHTTPRESPONSE_H
#define PARSEHTTPRESPONSE_H
#include <functional>
#include <map>
#include "HttpRequest.h"
#include "HttpResponse.h"
#include "ParseHttpRequest.h"
#include "Timer.h"
#include "Socket.h"
class ParseHttpResponse
{

public:
      ParseHttpResponse(std::shared_ptr<ClientSocket> client_socket_ptr,
                        std::shared_ptr<HttpRequest> http_request_ptr,
                        std::shared_ptr<HttpResponse> http_response_ptr,
                        std::weak_ptr<TimerNode> timer_ptr) {}
      const static std::unordered_map<std::string, std::function<void(ParseHttpResponse *)>> KPostUrlFuncMap;

      void appendStateLine();
      void appendHeader();
      void appendBody();
      void parse();
      void writeResponse();
      static void postLog(ParseHttpResponse *);
      static void postRegister(ParseHttpResponse *);

private:
      std::string ErrorPath;
      std::shared_ptr<ClientSocket> client_ptr_;
      std::shared_ptr<HttpRequest> http_request_ptr_;
      std::shared_ptr<HttpResponse> http_response_ptr_;
      std::weak_ptr<TimerNode> timer_ptr_;
};
#endif
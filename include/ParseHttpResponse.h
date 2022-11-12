#ifndef PARSEHTTPRESPONSE_H
#define PARSEHTTPRESPONSE_H
#include <functional>
#include <sys/stat.h>
#include "HttpRequest.h"
#include "HttpResponse.h"
#include "ParseHttpRequest.h"
#include "Timer.h"
#include "Socket.h"

const static std::string KBasePath;
class ParseHttpResponse
{

public:
      ParseHttpResponse(std::shared_ptr<ClientSocket> client_socket_ptr,
                        std::shared_ptr<HttpRequest> http_request_ptr,
                        std::shared_ptr<HttpResponse> http_response_ptr,
                        std::weak_ptr<TimerNode> timer_ptr) {}
      const static std::unordered_map<std::string, std::function<void(ParseHttpResponse *)>> KPostUrlFuncMap;
      const static std::unordered_map<std::string,std::string> KErrorPagePathMap;

      void appendStateLine();
      void appendHeader();
      void appendBody();
      void parse();
      void writeResponse();
      void checkFile();
      void checkInterface();
      static void postLog(ParseHttpResponse *);
      static void postRegister(ParseHttpResponse *);

private:
      //如果erro_path_被指定，则使用erro_path_对应资源回复
      std::string error_path_;
      std::shared_ptr<ClientSocket> client_ptr_;
      std::shared_ptr<HttpRequest> http_request_ptr_;
      std::shared_ptr<HttpResponse> http_response_ptr_;
      std::weak_ptr<TimerNode> timer_ptr_;
};
#endif
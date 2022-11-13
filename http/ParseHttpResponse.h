#ifndef PARSEHTTPRESPONSE_H
#define PARSEHTTPRESPONSE_H
#include <functional>
#include <sys/stat.h>
#include "HttpRequest.h"
#include "HttpResponse.h"
#include "ParseHttpRequest.h"
#include "Socket.h"

const static std::string KBasePath;
class ParseHttpResponse
{

public:
      ParseHttpResponse(std::shared_ptr<HttpResponse> &http_response_ptr, std::shared_ptr<ClientSocket> &client_socket_ptr);
      const static std::unordered_map<std::string, std::string> KFileTypeMap;
      const static std::unordered_map<std::string, std::string> KErrorPagePathMap;
      const static std::string KLineEnd;
      const static std::string KKeyValueSeparate;
      const static std::string KSpace;
      static void appendStateLine(std::shared_ptr<HttpResponse> &);
      static void appendHeader(std::shared_ptr<HttpResponse> &);
      static void appendBody(std::shared_ptr<HttpResponse> &);
      static void parse(std::shared_ptr<HttpResponse> &, std::shared_ptr<ClientSocket> &);
      static void writeResponse(std::shared_ptr<HttpResponse> &, std::shared_ptr<ClientSocket> &);
      static bool checkFile(std::string &);
      static std::string getFileType(std::string &);
      static void checkInterface();

private:
      //如果erro_path_被指定，则使用erro_path_对应资源回复
      std::string m_error_path_;
      std::shared_ptr<ClientSocket> & m_client_ptr_;
      std::shared_ptr<HttpResponse> & m_http_response_ptr_;
};
#endif
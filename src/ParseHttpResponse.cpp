//
// Created by yonggeng on 3/26/22.
//

#include "../include/ParseHttpResponse.h"


char NOT_FOUND_PAGE[] = "<html>\n"
                        "<head><title>404 Not Found</title></head>\n"
                        "<body bgcolor=\"white\">\n"
                        "<center><h1>404 Not Found</h1></center>\n"
                        "<hr><center>RunningBeef WebServer (Linux)</center>\n"
                        "</body>\n"
                        "</html>";

char FORBIDDEN_PAGE[] = "<html>\n"
                        "<head><title>403 Forbidden</title></head>\n"
                        "<body bgcolor=\"white\">\n"
                        "<center><h1>403 Forbidden</h1></center>\n"
                        "<hr><center>RunningBeef WebServer (Linux)</center>\n"
                        "</body>\n"
                        "</html>";

char INDEX_PAGE[] = "<!DOCTYPE html>\n"
                    "<html>\n"
                    "<head>\n"
                    "    <title>Welcome to RunningBeef WebServer!</title>\n"
                    "    <style>\n"
                    "        body {\n"
                    "            width: 35em;\n"
                    "            margin: 0 auto;\n"
                    "            font-family: Tahoma, Verdana, Arial, sans-serif;\n"
                    "        }\n"
                    "    </style>\n"
                    "</head>\n"
                    "<body>\n"
                    "<h1>Welcome to RunningBeef WebServer!</h1>\n"
                    "<p>If you see this page, the lc webserver is successfully installed and\n"
                    "    working. </p>\n"
                    "\n"
                    "<p>For online documentation and support please refer to\n"
                    "    <a href=\"https://github.com/RunningBeef/WebServer\">RunningBeef WebServer</a>.<br/>\n"
                    "\n"
                    "<p><em>Thank you for using RunningBeef WebServer.</em></p>\n"
                    "</body>\n"
                    "</html>";


void ParseHttpResponse::header(std::shared_ptr<HttpData> httpData){
    if(httpData->sharedPtr_httpRequest->getVersion() == HttpRequest::HTTP1_1){
        httpData->sharedPtr_httpResponse->setVersion(HttpRequest::HTTP1_1);
    }else {
        httpData->sharedPtr_httpResponse->setVersion(HttpRequest::HTTP1_0);
    }
    httpData->sharedPtr_httpResponse->addHeader("Server", "RunningBeef WebServer");
}

ParseHttpResponse::FILE_STATUS ParseHttpResponse::get_file(std::shared_ptr<HttpData> httpData, const char *basePath){
    struct stat file_stat;
    char file[strlen(basePath) + strlen(httpData->sharedPtr_httpResponse->filePath().c_str())+1];
    strcpy(file, basePath);
    strcat(file, httpData->sharedPtr_httpResponse->filePath().c_str());

    // 文件不存在
    if (httpData->sharedPtr_httpResponse->filePath() == "/" || stat(file, &file_stat) < 0) {
        // FIXME 设置Mime 为 html
        httpData->sharedPtr_httpResponse->setMime(MimeType("text/html"));
        if (httpData->sharedPtr_httpResponse->filePath() == "/") {
            httpData->sharedPtr_httpResponse->setStatusCode(HttpResponse::k200Ok);
            httpData->sharedPtr_httpResponse->setStatusMsg("OK");
        } else {
            httpData->sharedPtr_httpResponse->setStatusCode(HttpResponse::k404NotFound);
            httpData->sharedPtr_httpResponse->setStatusMsg("Not Found");
        }
        // 废弃， 404就不需要设置filepath
        //httpData->sharedPtr_httpResponse->setFilePath(std::string(basepath)+"/404.html");
        //std::cout << "File Not Found: " <<   file << std::endl;
        return ParseHttpResponse::FILE_STATUS::FILE_NOT_FOUND;
    }
    // 不是普通文件或无访问权限
    if(!S_ISREG(file_stat.st_mode)){
        // FIXME 设置Mime 为 html
        httpData->sharedPtr_httpResponse->setMime(MimeType("text/html"));
        httpData->sharedPtr_httpResponse->setStatusCode(HttpResponse::k403forbiden);
        httpData->sharedPtr_httpResponse->setStatusMsg("ForBidden");
        // 废弃， 403就不需要设置filepath
        //httpData->sharedPtr_httpResponse->setFilePath(std::string(basepath)+"/403.html");
        std::cout << "not normal file" << std::endl;
        return FILE_FORBIDDEN;
    }

    httpData->sharedPtr_httpResponse->setStatusCode(HttpResponse::k200Ok);
    httpData->sharedPtr_httpResponse->setStatusMsg("OK");
    httpData->sharedPtr_httpResponse->setFilePath(file);
//    std::cout << "文件存在 - ok" << std::endl;
    return FILE_OK;
}

void ParseHttpResponse:: getMime(std::shared_ptr<HttpData> httpData){
    std::string filepath = httpData->sharedPtr_httpRequest->getUrl();
    std::string mime;
    int pos;
//    std::cout << "uri: " << filepath << std::endl;
    // FIXME 直接将参数丢掉了，后续可以开发
    if ((pos = filepath.rfind('?')) != std::string::npos) {
        filepath.erase(filepath.rfind('?'));
    }

    if (filepath.rfind('.') != std::string::npos){
        mime = filepath.substr(filepath.rfind('.'));
    }
    decltype(Mime_map)::iterator it;

    if ((it = Mime_map.find(mime)) != Mime_map.end()) {
        httpData->sharedPtr_httpResponse->setMime(it->second);
    } else {
        httpData->sharedPtr_httpResponse->setMime(Mime_map.find("default")->second);
    }
    httpData->sharedPtr_httpResponse->setFilePath(filepath);
}

void ParseHttpResponse::send(std::shared_ptr<HttpData> httpData,FILE_STATUS fileStatus){
    char header[BUFFERSIZE];
    bzero(header, '\0');
    const char *internal_error = "Internal Error";
    struct stat file_stat;
    httpData->sharedPtr_httpResponse->appendBuffer(header);
    // 404
    if (fileStatus == FILE_NOT_FOUND) {

        // 如果是 '/'开头就发送默认页
        if (httpData->sharedPtr_httpResponse->filePath() == std::string("/")) {
            // 现在使用测试页面
            sprintf(header, "%sContent-length: %d\r\n\r\n", header, strlen(INDEX_PAGE));
            sprintf(header, "%s%s", header, INDEX_PAGE);
        } else {
            sprintf(header, "%sContent-length: %d\r\n\r\n", header, strlen(NOT_FOUND_PAGE));
            sprintf(header, "%s%s", header, NOT_FOUND_PAGE);
        }
        ::send(httpData->sharedPtr_clientSocket->fd_, header, strlen(header), 0);
        return;
    }

    if (fileStatus == FILE_FORBIDDEN) {
        sprintf(header, "%sContent-length: %d\r\n\r\n", header, strlen(FORBIDDEN_PAGE));
        sprintf(header, "%s%s", header, FORBIDDEN_PAGE);
        ::send(httpData->sharedPtr_clientSocket->fd_, header, strlen(header), 0);
        return;
    }
    // 获取文件状态
    if (stat(httpData->sharedPtr_httpResponse->filePath().c_str(), &file_stat) < 0) {
        sprintf(header, "%sContent-length: %d\r\n\r\n", header, strlen(internal_error));
        sprintf(header, "%s%s", header, internal_error);
        ::send(httpData->sharedPtr_clientSocket->fd_, header, strlen(header), 0);
        return;
    }

    int filefd = ::open(httpData->sharedPtr_httpResponse->filePath().c_str(), O_RDONLY);
    // 内部错误
    if (filefd < 0) {
        std::cout << "打开文件失败" << std::endl;
        sprintf(header, "%sContent-length: %d\r\n\r\n", header, strlen(internal_error));
        sprintf(header, "%s%s", header, internal_error);
        ::send(httpData->sharedPtr_clientSocket->fd_, header, strlen(header), 0);
        close(filefd);
        return;
    }

    sprintf(header,"%sContent-length: %d\r\n\r\n", header, file_stat.st_size);
    ::send(httpData->sharedPtr_clientSocket->fd_, header, strlen(header), 0);
    void *mapbuf = mmap(NULL, file_stat.st_size, PROT_READ, MAP_PRIVATE, filefd, 0);
    ::send(httpData->sharedPtr_clientSocket->fd_, mapbuf, file_stat.st_size, 0);
    munmap(mapbuf, file_stat.st_size);
    close(filefd);
    return;
    err:
    sprintf(header, "%sContent-length: %d\r\n\r\n", header, strlen(internal_error));
    sprintf(header, "%s%s", header, internal_error);
    ::send(httpData->sharedPtr_clientSocket->fd_, header, strlen(header), 0);
    return;
}
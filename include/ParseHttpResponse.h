//
// Created by yonggeng on 3/26/22.
//

#ifndef RUNNINGBEEF_WEBSERVER_PARSEHTTPRESPONSE_H
#define RUNNINGBEEF_WEBSERVER_PARSEHTTPRESPONSE_H

#include "HttpData.h"
#include<memory>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>

#define  BUFFERSIZE 2048


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
                    "    <title>Welcome to LC WebServer!</title>\n"
                    "    <style>\n"
                    "        body {\n"
                    "            width: 35em;\n"
                    "            margin: 0 auto;\n"
                    "            font-family: Tahoma, Verdana, Arial, sans-serif;\n"
                    "        }\n"
                    "    </style>\n"
                    "</head>\n"
                    "<body>\n"
                    "<h1>Welcome to LC WebServer!</h1>\n"
                    "<p>If you see this page, the lc webserver is successfully installed and\n"
                    "    working. </p>\n"
                    "\n"
                    "<p>For online documentation and support please refer to\n"
                    "    <a href=\"https://github.com/RunningBeef/WebServer\">RunningBeef WebServer</a>.<br/>\n"
                    "\n"
                    "<p><em>Thank you for using RunningBeef WebServer.</em></p>\n"
                    "</body>\n"
                    "</html>";

class HttpData;
class ParseHttpResponse{
public:
    enum FILE_STATUS{
        FILE_OK,
        FILE_NOT_FOUND,
        FILE_FORBIDDEN
    };
public:
    static void header(std::shared_ptr<HttpData>);
    static FILE_STATUS get_file(std::shared_ptr<HttpData>,const char * );
    static void getMime(std::shared_ptr<HttpData>);
    static void send(std::shared_ptr<HttpData>,FILE_STATUS);

};
#endif //RUNNINGBEEF_WEBSERVER_PARSEHTTPRESPONSE_H

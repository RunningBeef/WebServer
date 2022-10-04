//
// Created by yonggeng on 3/26/22.
//

#ifndef RUNNINGBEEF_WEBSERVER_PARSEHTTPRESPONSE_H
#define RUNNINGBEEF_WEBSERVER_PARSEHTTPRESPONSE_H
// #pragma once

extern char NOT_FOUND_PAGE[];
extern char FORBIDDEN_PAGE[];
extern char INDEX_PAGE[];


#include "HttpData.h"
#include <memory>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <iostream>

#define  BUFFERSIZE 6096



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
#endif
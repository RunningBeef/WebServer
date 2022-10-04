//
// Created by yonggeng on 3/24/22.
//

#ifndef RUNNINGBEEF_WEBSERVER_PARSEHTTPREQUEST_H
#define RUNNINGBEEF_WEBSERVER_PARSEHTTPREQUEST_H
// #pragma once

#define CR '\r'
#define LF '\n'
#define STRING_END '\0'

#include "../include/HttpRequest.h"
#include<string.h>
#include<iostream>


class ParseHttpRequest{
public:
    /*从状态机的三种可能状态, 即行的读取状态, 分别表示：读取到一个完整的行、行出错和行数据尚且不完整*/
    enum LINE_STATUS{
        LINE_OK,LINE_MORE,LINE_BAD
    };
    /*主状态机的两种可能状态, 分别表示：当前正在分析请求行, 当前正在分析头部字段*/
    enum PARSE_STATUS{
        PARSE_REQUESTLINE,PARSE_HEADER,PARSE_BODY
    };
    /*服务器处理HTTP请求的结果：
     * NO_REQUEST表示请求不完整, 需要继续读取客户数据；
     * GET_REQUEST表示获得了一个完整的客户请求；
     * BAD_REQUEST表示客户请求有语法错误；
     * FORBIDDEN_REQUEST表示客户对资源没有足够的访问权限；
     * INTERNAL_ERROR表示服务器内部错误；
     * CLOSED_CONNECTION表示客户端已经关闭连接了
     * */
    enum HTTP_CODE{NO_REQUEST,GET_REQUEST,BAD_REQUEST,
            FORBIDDEN_REQUEST,INTERNAL_ERROR,CLOSED_CONNECTION};


    /*0 ~ check_index - 1已经处理完了，check_index ~ read_index - 1还没处理*/
    static LINE_STATUS get_line(char * buffer,int & checked_index,int & read_index);
    static HTTP_CODE parse_requestLine(char * buffer,PARSE_STATUS & parseStatus,HttpRequest & httpRequest);
    static HTTP_CODE parse_header(char * buffer,PARSE_STATUS & parseStatus,HttpRequest & httpRequest);
    static HTTP_CODE parse_body(char *body, HttpRequest &request);

    /*check_index 起始位置， read_index数据结尾位置的下一位，next_line_index记录下一行起始位置*/
    static HTTP_CODE pare_httpRequest(char * buffer,int & checked_index, int & read_index,int & next_line_index,PARSE_STATUS &parseStatus,HttpRequest & httpRequest);
};
#endif
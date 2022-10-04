//
// Created by yonggeng on 3/22/22.
//

#ifndef RUNNINGBEEF_WEBSERVER_SOCKET_H
#define RUNNINGBEEF_WEBSERVER_SOCKET_H
// #pragma once

#include<string.h>
#include<iostream>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<assert.h>
#include<unistd.h>
#include<stdlib.h>
#include<errno.h>
#include<fcntl.h>

class ClientSocket;

/* 将socket设置为非阻塞 */
int setNonBlocking(int fd) ;

/* 服务端socket */
class ServerSocket
{
public:
    ServerSocket(int port = 8080,char * ip  = NULL);
    ~ServerSocket();
    void bind();
    void listen();
    int accept(ClientSocket &clientSocket);
    void setSocketReused();
public:
    int port_;
    char * ip_;
    int server_socket_fd_;/* 服务端socket fd*/
    struct sockaddr_in sockAddr_;/* 服务端socket 协议簇、ip地址、端口号*/
};

/* 客户端socket */
class ClientSocket
{
public:
    ClientSocket();
    ~ClientSocket();
    ClientSocket(const ClientSocket &clientSocket);
    ClientSocket & operator = (ClientSocket clientSocket);
public:
    int fd_;/*接受的socket连接*/
    int epoll_fd_;/*指向监听内核事件的内核事件表*/
    socklen_t clientAddrlength_;/*保存客户端信息*/
    struct sockaddr_in clientAddr_;/*accept时指出远端socket地址长度*/

};


#endif
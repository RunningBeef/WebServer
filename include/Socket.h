//
// Created by yonggeng on 3/22/22.
//

#ifndef RUNNINGBEEF_WEBSERVER_SOCKET_H
#define RUNNINGBEEF_WEBSERVER_SOCKET_H
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

int setNonBlocking(int fd) ;

class ServerSocket
{
public:
    ServerSocket(int port = 8080,char * ip  = NULL);
    ~ServerSocket();
    void bind();
    void listen();
    void accept(ClientSocket &clientSocket);
    void setSocketReused();
public:
    int port_;
    char * ip_;
    int socket_fd_;
    int epoll_fd_;
    struct sockaddr_in sockAddr_;
};

class ClientSocket
{
public:
    ClientSocket();
    ~ClientSocket();
public:
    int fd_;/*接受的socket连接*/
    int epoll_fd_;/*指向监听内核事件的内核事件表*/
    socklen_t clientAddrlength_;/*保存客户端信息*/
    struct sockaddr_in clientAddr_;/*accept时指出远端socket地址长度*/

};


#endif //RUNNINGBEEF_WEBSERVER_SOCKET_H

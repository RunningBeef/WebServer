//
// Created by yonggeng on 3/23/22.
//
#include "../include/Socket.h"


/*函数声明时有默认参数，实现的时候就不能加默认参数*/
ServerSocket::ServerSocket(int port, char *ip) : port_(port), ip_(ip) {
    bzero(&sockAddr_, sizeof(sockAddr_));
    sockAddr_.sin_family = AF_INET;
    inet_pton(AF_INET, ip_, &sockAddr_.sin_addr);/*点分十进制ip地址转网络字节序保存在sockAdd_r指向内存中*/
    sockAddr_.sin_port = htons(port_);

    ServerSocket::socket_fd_ = socket(PF_INET, SOCK_STREAM, 0);
    setSocketReused();/*強制使用TIME_WAIT状态的socket*/
    bind();
    listen();
}

ServerSocket::~ServerSocket() {
    close(ServerSocket::socket_fd_);
}

void ServerSocket::bind() {
    if(::bind(ServerSocket::socket_fd_,(struct sockaddr *)&sockAddr_,sizeof(sockAddr_)) == -1)
    {
        std::cout << "error in file " << __FILE__ << " at line " << __LINE__ << std::endl;
        exit(1);

    }
}

void ServerSocket::listen(){
    if(::listen(socket_fd_,1024) == -1)
    {
        std::cout << "error in file " << __FILE__ << " at line " << __LINE__ << std::endl;
        exit(1);
    }
}

void ServerSocket::accept(ClientSocket & clientSocket){
    clientSocket.fd_ = ::accept(socket_fd_,(struct sockaddr * )&clientSocket.clientAddr_,&clientSocket.clientAddrlength_);
    if(clientSocket.fd_ < 0){
        std::cout << "error in file " << __FILE__ << " at line " <<  __LINE__ << std::endl;
        exit(1);
    }
    clientSocket.epoll_fd_ = ServerSocket::epoll_fd_;

}

void ServerSocket::setSocketReused()
{
    int reuse = 1;
    setsockopt(socket_fd_,SOL_SOCKET,SO_REUSEADDR,&reuse,sizeof(reuse));
}


ClientSocket::ClientSocket() {
    ClientSocket::clientAddrlength_ = sizeof(ClientSocket::clientAddr_);
}

ClientSocket::~ClientSocket() {
    close(ClientSocket::fd_);
}




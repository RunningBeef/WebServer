//
// Created by yonggeng on 3/23/22.
//
#include "../include/Socket.h"

int setNonBlocking(int fd)
{
    int old_option = fcntl(fd, F_GETFL);
    int new_option = old_option | O_NONBLOCK;
    fcntl(fd, F_SETFL, new_option);
    return old_option;
}

/*函数声明时有默认参数，实现的时候就不能加默认参数*/
ServerSocket::ServerSocket(int port, char *ip) : port_(port), ip_(ip)
{
    bzero(&sockAddr_, sizeof(sockAddr_));
    sockAddr_.sin_family = AF_INET;

    /* ip地址转换 */
    if (ip_ != nullptr)
    {
        inet_pton(AF_INET, ip_, &sockAddr_.sin_addr); /*点分十进制ip地址转网络字节序保存在sockAdd_r指向内存中*/
    }
    else
    {
        sockAddr_.sin_addr.s_addr = htonl(INADDR_ANY);
    }

    /* 端口号转换 */
    sockAddr_.sin_port = htons(port_);

    ServerSocket::server_socket_fd_ = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket_fd_ == -1)
    {
        std::cout << "error in file " << __FILE__ << " at line " << __LINE__ << std::endl;
        throw ::std::exception();
    }

    setSocketReused();                 /*強制使用TIME_WAIT状态的socket*/
    setNonBlocking(server_socket_fd_); /*设置为非阻塞*/

    bind();   /* 命名 */
    listen(); /* 设置监听队列大小 */
}

ServerSocket::~ServerSocket()
{
    close(ServerSocket::server_socket_fd_);
}

void ServerSocket::bind()
{
    if (::bind(ServerSocket::server_socket_fd_, (struct sockaddr *)&sockAddr_, sizeof(sockAddr_)) == -1)
    {
        std::cout << "bind file in file " << __FILE__ << " at line " << __LINE__ << std::endl;
        exit(1);
    }
}

void ServerSocket::listen()
{
    if (::listen(server_socket_fd_, 1024) == -1)
    {
        std::cout << "listen file in file " << __FILE__ << " at line " << __LINE__ << std::endl;
        exit(1);
    }
}

/* 返回从监听队列取出的fd */
int ServerSocket::accept(ClientSocket &clientSocket)
{
    // std::cout << "socket_fd_: " << server_socket_fd_ << std::endl;
    clientSocket.fd_ = ::accept(server_socket_fd_, (struct sockaddr *)&clientSocket.clientAddr_, &clientSocket.clientAddrlength_);
    // clientSocket.fd_ = ::accept(socket_fd_,NULL,NULL);

    if (clientSocket.fd_ < 0)
    {
        // std::cout << "error in file " << __FILE__ << " at line " <<  __LINE__ << " because accept return -1"<< std::endl;
        return -1;
        // throw::std::exception();
    }

    return clientSocket.fd_;
}

void ServerSocket::setSocketReused()
{
    int reuse = 1;
    setsockopt(server_socket_fd_, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));
}

ClientSocket::ClientSocket()
{
    ClientSocket::clientAddrlength_ = sizeof(ClientSocket::clientAddr_);
}

ClientSocket::~ClientSocket()
{
    std::cout<<"closer fd " << fd_ << "success in file " << __FILE__ << "at line " << __LINE__ << std::endl;
    close(ClientSocket::fd_);
}

ClientSocket::ClientSocket(const ClientSocket & clientSocket)
{
    this->clientAddr_ = clientSocket.clientAddr_;
    this->clientAddrlength_ = clientSocket.clientAddrlength_;
    this->fd_ = clientSocket.fd_;
}
ClientSocket &ClientSocket::operator=(ClientSocket clientSocket)
{
    this->clientAddr_ = clientSocket.clientAddr_;
    this->clientAddrlength_ = clientSocket.clientAddrlength_;
    this->fd_ = clientSocket.fd_;
    return *this;
}

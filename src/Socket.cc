#include "../include/Socket.h"

ServerSocket::
    ServerSocket(const char *ip = NULL, int port = 80, int backlog = 1024)
    : ip_(ip), port_(port), backlog_(backlog), server_sockfd_(-1)
{
      socket();
      reuseAddr();
      bind();
      listen();
}

ServerSocket::~ServerSocket()
{
      if (server_sockfd_ != -1)
            close(server_sockfd_);
}

void ServerSocket::socket()
{
      server_sockfd_ = ::socket(PF_INET, SOCK_STREAM, 0); //::调用全局socket, PF_INET协议簇
      assert(server_sockfd_ >= 0);
}

void ServerSocket::reuseAddr()
{
      int reuse = 1;
      setsockopt(server_sockfd_, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));
}

void ServerSocket::bind()
{
      bzero(&server_address_, sizeof(server_address_));
      server_address_.sin_family = AF_INET; // TCP/IPV4地址簇

      if (ip_ != NULL)
            inet_pton(AF_INET, ip_, &server_address_.sin_addr);
      else
            server_address_.sin_addr.s_addr = htonl(INADDR_ANY); //绑定地址0.0.0.0，能收到任意网卡/IP的连接
      server_address_.sin_port = htons(port_);

      int ret = ::bind(server_sockfd_, (struct sockaddr *)(&server_address_), sizeof(server_address_));
#ifdef DEBUG
      printf("%s\n", strerror(errno)); // EACCES 0 ~ 1023受保护，EADDRINUSE绑定到处于TIMEWAIT状态的SOCKET地址
#endif
      assert(ret != -1);
}

//创建监听队列
void ServerSocket::listen()
{
      int ret = ::listen(server_sockfd_, backlog_);
#ifdef DEBUG
      printf("%s\n", strerror(errno));
#endif;
      assert(ret != -1);
}

void ServerSocket::accept(ClientSocket &ClientSocket)
{
      ClientSocket.client_sockfd_ = ::accept(server_sockfd_,
                                             (struct sockaddr *)(&ClientSocket.client_address_),
                                             &ClientSocket.client_addr_length_);
}

ClientSocket::ClientSocket() : client_sockfd_(-1)
{
}
ClientSocket::~ClientSocket()
{
      if (client_sockfd_ != -1)
            close(client_sockfd_);
}

void ClientSocket::setRecvBuffSize()
{
      int buff_size = RCV_BUF_SIZE;
      setsockopt(client_sockfd_, SOL_SOCKET, SO_RCVBUF, &buff_size, sizeof(buff_size));
}

void ClientSocket::setSendBuffSize()
{
      int buff_size = SND_BUF_SIZE;
      setsockopt(client_sockfd_, SOL_SOCKET, SO_SNDBUF, &buff_size, sizeof(buff_size));
}
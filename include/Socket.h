#ifndef SOCKET_H
#define SOCKET_H
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>
#define SND_BUF_SIZE 5120
#define RCV_BUF_SIZE 1024

class ClientSocket;

class ServerSocket
{
public:
      ServerSocket(const char *ip = NULL, int port = 80, int backlog = 1024);
      ~ServerSocket();
      void socket();
      void bind();
      void listen();
      void accept(ClientSocket &);
      void reuseAddr();

private:
      int port_;
      const char *ip_;
      int server_sockfd_;
      int backlog_;
      struct sockaddr_in server_address_;
      // the port and ip address of sockaddr_in should be netWork byte order
};

class ClientSocket
{
      friend void ServerSocket::accept(ClientSocket &);

public:
      ClientSocket();
      ~ClientSocket();
      void setSendBuffSize(); //记得修改发送缓冲区大小
      void setRecvBuffSize();

private:
      int client_sockfd_;
      struct sockaddr_in client_address_;
      socklen_t client_addr_length_;
};

#endif
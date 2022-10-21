#ifndef SOCKET_H
#define SOCKET_H
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>
class ClientSocket;

class ServerSocket
{
public:
      ServerSocket(const char *ip = NULL, int port = 80,int backlog = 1024);
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

private:
      int client_sockfd_;
      struct sockaddr_in client_address_;
      socklen_t client_addr_length_;
};

#endif
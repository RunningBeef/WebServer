#ifndef EPOLL_H
#define EPOLL_H

#include <sys/epoll.h>
#include <fcntl.h>
#include <assert.h>
#include <exception>

#define MIN_EVENT_SIZE 1024
#define MAX_EVENT_SIZE 20480

// listen_fd EPOLL_ET | EPOLL_IN
// client_fd EPOLL_ET | EPOLL_IN | EPOLL_OENSHOUT  ? EPOLL_OUT
class Epoll
{
public:
      Epoll(int);
      ~Epoll();
      int wait(int timeout);
      void addFd(int fd, struct epoll_event *event);
      void delFd(int fd, struct epoll_event *event);
      void modFd(int fd, struct epoll_event *event);
      int setNonBlocking(int);

private:
      int epoll_fd_;
      int event_size_;
      struct epoll_event *events_;
};
#endif
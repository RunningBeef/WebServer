#include "../include/Epoll.h"

Epoll::Epoll(int event_size):event_size_(event_size){

      epoll_fd_ = epoll_create(1);
      //只要大于 0 即可
      if(event_size_ < MIN_EVENT_SIZE)
            event_size = MIN_EVENT_SIZE;
      else if(event_size > MAX_EVENT_SIZE)
            event_size = MAX_EVENT_SIZE;
      events_ = new epoll_event[event_size];
      if(events_ == nullptr){
            throw std::exception();
      }
}

Epoll::~Epoll(){
      if(events_ != nullptr)
            delete[] events_;
}

int Epoll::wait(int timeout){
      return epoll_wait(epoll_fd_,events_,event_size_,timeout);
}

void Epoll::addFd(int fd, epoll_event * event){
      int ret = epoll_ctl(epoll_fd_,EPOLL_CTL_ADD,fd,event);
      setNonBlocking(fd);
      assert(ret == 0);
}

void Epoll::modFd(int fd, epoll_event * event){
      int ret = epoll_ctl(epoll_fd_, EPOLL_CTL_MOD, fd, event);
      assert(ret == 0);
}
void Epoll::delFd(int fd, epoll_event * event){
      int ret = epoll_ctl(epoll_fd_, EPOLL_CTL_DEL, fd, event);
      assert(ret == 0);
}

int Epoll::setNonBlocking(int fd){
      int old_option = fcntl(fd,F_GETFL);
      int new_option = old_option | O_NONBLOCK;
      fcntl(fd,F_SETFL,new_option);
      return old_option;
}


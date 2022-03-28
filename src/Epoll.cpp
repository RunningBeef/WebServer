//
// Created by yonggeng on 3/25/22.
//

#include "../include/Epoll.h"

__uint32_t Epoll:: DEFAULT_EVENT = EPOLLIN | EPOLLET | EPOLLONESHOT;

Epoll::Epoll(int size):events_(nullptr){
    max_events_ = 1024 > size ? 1024 : size;/*设置监听事件数组大小*/
    epoll_fd_ = epoll_create(max_events_);
    events_ = new epoll_event[max_events_];
}

Epoll::~Epoll() {
    if (events_ != nullptr)
        delete events_;
}

int Epoll::modFd(int op,int fd,struct epoll_event *event){

    return epoll_ctl(epoll_fd_,op,fd,event);

}

int Epoll::wait(int timeout){

    return epoll_wait(epoll_fd_,events_,max_events_,timeout);
}

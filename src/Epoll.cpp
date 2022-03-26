//
// Created by yonggeng on 3/25/22.
//

#include "../include/Epoll.h"

__uint32_t Epoll:: DEFAULT_EVENT = EPOLLIN | EPOLLET | EPOLLONESHOT;

Epoll::Epoll(int size):events(nullptr){
    max_events = 1024 > size ? 1024 : size;/*设置监听事件数组大小*/
    epoll_fd = epoll_create(max_events);
    events = new epoll_event[max_events];
}
Epoll::~Epoll() {
    if (events != nullptr)
        delete events;
}

int Epoll::modFd(int op,int fd,struct epoll_event *event){
    return epoll_ctl(epoll_fd,op,fd,event);
}

int Epoll::wait(struct epoll_event * events ,int timeout){

    return epoll_wait(epoll_fd,events,max_events,timeout);
}
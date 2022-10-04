//
// Created by yonggeng on 3/25/22.
//

#ifndef RUNNINGBEEF_WEBSERVER_EPOLL_H
#define RUNNINGBEEF_WEBSERVER_EPOLL_H
// #pragma once

#include <sys/epoll.h>
#include <memory>
#include <vector>
#include "Socket.h"
#include "HttpData.h"
#include "Noncopyable.h"

class Epoll : public Noncopyable{
public:
    Epoll(int size);
    ~Epoll();
    /*对监听事件增删改查*/
    int modFd(int op,int fd,struct epoll_event *);
    /*调用epoll_wait*/
    int wait(int timeout);
    /*默认监听事件*/
    static  __uint32_t DEFAULT_EVENT;
    /* 放回对应的内核事件表fd */
    int get_epoll_fd(){return epoll_fd_;}
    /* 放回监听到的就绪事件 */
    struct epoll_event * getEpollEvent(){return events_;}
private:
    /*就绪事件列表大小*/
    int max_events_;
    /*保存epoll_create()返回的内核事件表文件描述符*/
    int epoll_fd_;
    /*保存监听到的就绪事件*/
    struct epoll_event * events_;
};
#endif

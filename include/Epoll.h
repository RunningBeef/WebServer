//
// Created by yonggeng on 3/25/22.
//

#ifndef RUNNINGBEEF_WEBSERVER_EPOLL_H
#define RUNNINGBEEF_WEBSERVER_EPOLL_H
#include <sys/epoll.h>

class Epoll{
public:
    Epoll(int size);
    ~Epoll();
    int modFd(int op,int fd,struct epoll_event *);/**/
    int wait(struct epoll_event * ,int timeout);/*调用epoll_wait*/
    static  __uint32_t DEFAULT_EVENT;/*默认监听事件*/
private:
    int max_events;/*就绪事件列表大小*/
    int epoll_fd;/*保存epoll_create()返回的内核事件表文件描述符*/
    struct epoll_event * events;/*保存监听到的就绪事件*/
};
#endif //RUNNINGBEEF_WEBSERVER_EPOLL_H

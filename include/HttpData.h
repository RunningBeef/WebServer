//
// Created by yonggeng on 3/24/22.
//
//
//#ifndef RUNNINGBEEF_WEBSERVER_HTTPDATA_H
//#define RUNNINGBEEF_WEBSERVER_HTTPDATA_H
#pragma once

#include "Timer.h"
#include "HttpRequest.h"
#include "HttpResponse.h"
#include "Socket.h"
#include <memory>

class TimerNode;

class HttpData : public std::enable_shared_from_this<HttpData>{
public:
    HttpData(ClientSocket & );
    ~HttpData();
    void closeTimerNode();/*socket异常或者超时，调用TimerNode的回调函数，立刻关闭map中fd*/
    /*socket更新时，不方便从堆中直接找出这个定时器删除，
     * 而是先断开HttpData和旧的TimerNode的联系
     * 再给这个SOCKET建立新的TimerNode*/
    void breakRelated();/*断开HttpData和旧的TimerNode的联系*/
    void setTimerNode(std::shared_ptr<TimerNode> &);/*设置的TimerNode*/

public:
    std::shared_ptr<HttpRequest> sharedPtr_httpRequest;
    std::shared_ptr<HttpResponse> sharedPtr_httpResponse;
    std::shared_ptr<ClientSocket> sharedPtr_clientSocket;
    int epoll_fd_;
private:
    std::weak_ptr<TimerNode> weakPtr_TimerNode;

};
//#endif //RUNNINGBEEF_WEBSERVER_HTTPDATA_H

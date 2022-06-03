//
// Created by yonggeng on 3/25/22.
//

//#ifndef RUNNINGBEEF_WEBSERVER_TIMER_H
//#define RUNNINGBEEF_WEBSERVER_TIMER_H
#pragma once

#include "HttpData.h"
#include "Mutex.h"
#include "WebServer.h"
#include <sys/time.h>
#include <queue>
#include <deque>
#include <memory>
#include <functional>

class HttpData;
class WebServer;

struct CbTask{/*封装工作任务*/
    std::function<void(std::shared_ptr<void>)> function;/* 函数 */
    std::shared_ptr<void> arg;/*参数*/
};


class TimerNode{
public:
    TimerNode(size_t interval,CbTask task);
    ~TimerNode();
    bool isExpired();/*是否超时*/
    bool isProcessed();/*是否因为异常或者连接更新，被标记为删除*/

    void setExpiredTime(size_t interval);/*设置过期时间*/

    size_t getExpiredTime();/*获取过期时间*/

    void setProcessed();/*标记为任务已经处理*/

    static void updateCurrentTime();/*更新系统当前时间*/

public:
    static size_t current_src;/*系统当前时间*/
    static size_t DEFAULT_INTERVAL_SEC;/*默认超时时间*/
    // void (* cbFunc_ )(WebServer * , std::shared_ptr<HttpData>);/*回调函数*/
    // WebServer * webserver_;
    CbTask cbTask_;/* 代替原有的cbFunc_ */
private:

    bool processed_;/*定时任务是否已经处理了*/
    size_t expiredTime_;/*超时时间*/

};

struct TimerCmp{
    bool operator()(std::shared_ptr<TimerNode> & a,std::shared_ptr<TimerNode> & b)const {
        return a->getExpiredTime() < b->getExpiredTime();
    }
};
/*定时器容器*/
class TimerManager{
public:

    /*给堆添加定时器*/
    void addTimerNode(std::shared_ptr<TimerNode> timerNode);
    void tick();

private:
    Mutex mutex_;
    std::priority_queue<std::shared_ptr<TimerNode>, std::vector<std::shared_ptr<TimerNode>>, TimerCmp> timerQueue;
};
//#endif //RUNNINGBEEF_WEBSERVER_TIMER_H

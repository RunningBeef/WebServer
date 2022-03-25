//
// Created by yonggeng on 3/25/22.
//
#include "../include/Timer.h"
#include <sys/time.h>

size_t TimerNode:: DEFAULT_INTERVAL_SEC = 20;
TimerNode::TimerNode(size_t interval,std::shared_ptr<HttpData> httpData,void (*cbFunc)(std::shared_ptr<HttpData>))
:httpData_(httpData),deleted_(false),cbFunc_(cbFunc){
    TimerNode::setExpiredTime(TimerNode::DEFAULT_INTERVAL_SEC);
}

/*调用回调函数关闭socket...等资源*/
TimerNode::~TimerNode(){
    if(httpData_){
        cbFunc_(httpData_);
    }
}

/*是否超时*/
bool TimerNode::isExpired(){
    return expiredTime_ < TimerNode::current_src;
}

/*是否因为异常或着连接更新，被标记为删除*/
bool TimerNode:: isDeleted(){
    return deleted_;
}

/*设置过期时间*/
void TimerNode::setExpiredTime(size_t interval = DEFAULT_INTERVAL_SEC){
    updateCurrentTime();
    TimerNode::expiredTime_ = TimerNode::current_src + interval;
}

size_t TimerNode::getExpiredTime() {
    return expiredTime_;
}

void TimerNode::setDeleted(){
    deleted_ = true;
}

/*更新系统当前时间*/
void TimerNode::updateCurrentTime(){
    struct  timeval cur;
    gettimeofday(&cur,NULL);
    TimerNode::current_src = cur.tv_sec;
}


void TimerManager::addTimerNode(std::shared_ptr<HttpData> & httpData,size_t interval,void (* cbFunc )(std::shared_ptr<HttpData>)) {
    std::shared_ptr<TimerNode> timerNode(new TimerNode(interval,httpData,cbFunc));
    {
        MutexGuard mutexGuard(mutex_);/*上锁*/
        timerQueue.push(timerNode);
        httpData->setTimerNode(timerNode);
    }
}

void TimerManager::tick() {
    TimerNode::updateCurrentTime();
    MutexGuard mutexGuard(mutex_);/*上锁*/
    while(!timerQueue.empty()){
        std::shared_ptr<TimerNode> temp = timerQueue.top();
        if(temp->isExpired() || temp->isDeleted())
        {
            timerQueue.pop();
        }else break;
    }
}




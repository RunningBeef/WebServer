//
// Created by yonggeng on 3/25/22.
//
#include "../include/Timer.h"

size_t TimerNode::current_src = 0;
size_t TimerNode:: DEFAULT_INTERVAL_SEC = 100000;
TimerNode::TimerNode(size_t interval,CbTask task)
:processed_(false),cbTask_(task){
    TimerNode::setExpiredTime(TimerNode::DEFAULT_INTERVAL_SEC);
}

/*调用回调函数关闭socket...等资源*/
TimerNode::~TimerNode(){
    if(!isProcessed())/* 如果已经处理过就不处理了 */
        cbTask_.function(cbTask_.arg);
}

/*是否超时*/
bool TimerNode::isExpired(){
    return expiredTime_ < TimerNode::current_src;
}

/*是否因为异常或着连接更新，被标记为删除*/
bool TimerNode:: isProcessed(){
    return processed_;
}

/*设置过期时间*/
void TimerNode::setExpiredTime(size_t interval = DEFAULT_INTERVAL_SEC){
    updateCurrentTime();
    TimerNode::expiredTime_ = current_src + interval;
}

void TimerNode::setProcessed(){
    processed_ = true;
}

size_t TimerNode::getExpiredTime() {
    return expiredTime_;
}

/*更新系统当前时间*/
void TimerNode::updateCurrentTime(){
    struct  timeval cur;
    gettimeofday(&cur,NULL);
    TimerNode::current_src = cur.tv_sec;
}

/*向堆中添加定时器*/
void TimerManager::addTimerNode(std::shared_ptr<TimerNode> timerNode) {

    {
        MutexGuard mutexGuard(mutex_);/*上锁*/
        timerQueue.push(timerNode);
        // timerNode->httpData_->setTimerNode(timerNode);
    }
}

void TimerManager::tick() {
    TimerNode::updateCurrentTime();/*更新当前时间，方便判断socket是否超时*/
    MutexGuard mutexGuard(mutex_);/*上锁*/
    int ct = 0;
    while(!timerQueue.empty()){
        if(++ct == 1000){/*如果统一时刻大量事件超时，一直处理，会影响服务器响应其他socket的Http请求*/
            std::cout << "超时事件过多！！！" << std::endl;
            break;
        }
        std::shared_ptr<TimerNode> temp = timerQueue.top();
        if(temp->isExpired() || temp->isProcessed())
        {
            
            timerQueue.pop();
        }else break;
    }
}




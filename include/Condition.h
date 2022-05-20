//
// Created by yonggeng on 3/23/22.
//

//#ifndef RUNNINGBEEF_WEBSERVER_CONDITION_H
//#define RUNNINGBEEF_WEBSERVER_CONDITION_H
#pragma once

#include "../include/Noncopyable.h"
#include<pthread.h>
#include "../include/Mutex.h"


class Condition : public  Noncopyable{
public:
    Condition(Mutex &mutex);
    ~Condition();
    void notify();
    void notifyAll();
    void wait();
public:
    Mutex & mutex_;
    pthread_cond_t cond_;
};
//#endif //RUNNINGBEEF_WEBSERVER_CONDITION_H

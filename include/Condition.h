//
// Created by yonggeng on 3/23/22.
//

//#ifndef RUNNINGBEEF_WEBSERVER_CONDITION_H
//#define RUNNINGBEEF_WEBSERVER_CONDITION_H
#pragma once/* 防止重复头文件被重复引用，用#ifndef和#define出了点问题 */

#include "../include/Noncopyable.h"
#include<pthread.h>
#include "../include/Mutex.h"
/* 将条件变量和对应使用的一些方法封装成一个类，
禁止复制一个条件变量，将该类继承自Noncopyable类（禁用调拷贝构造和 拷贝函数）*/
class Condition : public  Noncopyable{
public:
    
    Condition(Mutex &mutex);
    ~Condition();
    /* 唤醒某个阻塞的工作线程，当然有惊群的可能 */
    void notify();
    /* 广播唤醒其条件变量等待队列里的所有线程*/
    void notifyAll();
    /* 尝试获取条件变量，如果值不为0则将值-1，否则线程睡眠，调用前一定要先上锁，具体见《linux高性能服务器编程》的条件变量*/
    void wait();
public:
    /* 调用wait方法使用的互斥锁*/
    Mutex & mutex_;

    pthread_cond_t cond_;
};
//#endif //RUNNINGBEEF_WEBSERVER_CONDITION_H

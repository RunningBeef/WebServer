//
// Created by yonggeng on 3/23/22.
//

#ifndef RUNNINGBEEF_WEBSERVER_MUTEX_H
#define RUNNINGBEEF_WEBSERVER_MUTEX_H
// #pragma once

#include "../include/Noncopyable.h"
#include<pthread.h>

/*继承Noncopyable类防止锁被无意间拷贝，
 * 给一个已经上锁的锁在次上锁，会一直阻塞下去
 * 或者一个已经解锁的锁解锁可能导致不可预期的结果
 * 因此我们delte了Mutex的拷贝函数和'='重载
 * 并且对锁的解锁和开锁和类的生命周期绑定在一起
 **/

class Mutex : public Noncopyable{
public:
    Mutex();
    ~Mutex();
    void lock();
    void unlock();
    pthread_mutex_t * getMutex();
public: pthread_mutex_t mutex_;
};

/*对锁的解锁和开锁和类的生命周期绑定在一起*/
class MutexGuard{
public:
    /* 要求显示声明的方式来构造 */
    explicit MutexGuard(Mutex &mutex);
    ~MutexGuard();
public:
    Mutex &mutex_;
};

#endif

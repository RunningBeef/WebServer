//
// Created by yonggeng on 3/26/22.
//

#ifndef RUNNINGBEEF_WEBSERVER_THREADPOOL_H
#define RUNNINGBEEF_WEBSERVER_THREADPOOL_H
#include "Mutex.h"
#include "Condition.h"
#include <exception>
#include <pthread.h>
#include <queue>
#include <vector>
#include <iostream>


template<class T>
class ThreadPool{
public:
    ThreadPool(int threadNum = 10,int maxTaskNum = 1024);
    ~ThreadPool();
    void appendTask(T * );
public:
    /*pthread运行的只能是static函数，
     * 但是你线程想运行的函数run()实际上会用的成员变量如task_queue_,
     * 这个你没法设成static因为它不能在线程池类中共享，task_queue_需要每个线程池类各自都有一个
     * 这个时候我们可以把线程池对象作为work()函数的参数传入
     * 然后在work函数内调用run()函数*/
    static void * work(void * arg);
    static int MAX_TASK_SIZE;/*任务队列中能存放的最大任务个数*/
    static int MAX_THREAD_NUM;/*线程池最大线程个数*/
    void run();
private:
    std::queue<T> task_queue_;/*任务队列*/
    std::vector<pthread_t > workThread_/*工作线程*/;
    int max_task_num_;/*最大任务个数*/
    int thread_num_;/*线程数量*/
    bool shutdown_;/*是否停止线程池，子线程所运行的run函数通过判断这个变量决定是否退出*/
    Mutex mutex_;
    Condition condition_;
};
#endif //RUNNINGBEEF_WEBSERVER_THREADPOOL_H

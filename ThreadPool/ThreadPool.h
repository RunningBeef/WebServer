#ifndef THREADPOOL_H
#define THREADPOOL_H

#include "../locker/Condition.h"
#include "../locker/Mutex.h"
#include <queue>
#include <pthread.h>
#include <exception>

#define MAX_THREAD 65;

/*
在C++程序中使用pthread_create函数时，该函数的第3个参数必须指向一个静态函数。
而要在一个静态函数中使用类的动态成员（包括成员函数和成员变量），则只能通过如下两种方式来实现：
❑通过类的静态对象来调用。比如单体模式中，静态函数可以通过类的全局唯一实例来访问动态成员函数。
❑将类的对象作为参数传递给该静态函数，然后在静态函数中引用这个对象，并调用其动态方法。
代码清单15-3使用的是第2种方式：将线程参数设置为this指针，然后在worker函数中获取该指针并调用其动态方法run。

摘自：《Linux高性能服务器编程》 — 游双
在豆瓣阅读书店查看：https://read.douban.com/ebook/15233070/
本作品由华章数媒授权豆瓣阅读中国大陆范围内电子版制作与发行。
© 版权所有，侵权必究。
 */
template <class T>
class ThreadPool
{
public:
      ThreadPool(int thread_num = 8, int max_request = 10000);
      //这里直接抛出异常，不会调用析构函数，会造成内存泄漏,后期想办法修改下
      ~ThreadPool();
      bool append(T &);
      static void worker(void *);
      // pthread_create()第三个参数必须指向一个静态函数，但是线程运行的函数是动态的
      // http://c.biancheng.net/view/8607.html
      void run();

private:
      int thread_num_;
      int max_request_;
      bool shutdown_;
      Condition request_condition_;
      Mutex queue_locker_;
      queue<T> request_queue_;
      pthread_t *threads_;
};
#endif
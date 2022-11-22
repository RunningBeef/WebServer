/*
 * @Author: RunningBeef 2723772192@qq.com
 * @Date: 2022-11-08 12:12:29
 * @LastEditors: RunningBeef 2723772192@qq.com
 * @LastEditTime: 2022-11-22 20:14:28
 * @FilePath: /lighthouse/WebServer/threadPool/ThreadPool.cc
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include "ThreadPool.h"
template <class T>
ThreadPool<T>::
    ThreadPool(int thread_num = 8, int max_request = 10000)
    : thread_num_(thread_num), max_request_(max_request),
      threads_(NULL), request_condition_(queue_locker_), shutdown_(false)
{
      if (thread_num_ <= 0 || max_request_ <= 0)
      {
            return;
      }
      if (thread_num_ > MAX_THREAD)
      {
            thread_num_ = MAX_THREAD;
      }

      threads_ = new pthread_t[thread_num_];
      if (!threads_)
      {
            return;
      }
      for (int i = 0; i < thread_num_; ++i)
      {
            if (pthread_create(threads + i, NULL, worker, this) || pthread_detach(threads + i))
            {
                  shutdown_ = true;
                  // https://blog.csdn.net/qq_33883085/article/details/89425933
                  delete[] threads_;
                  return;
            }
      }
}

/*
pthread_cond_wait函数执行时，首先把调用线程放入条件变量的等待队列中，然后将互斥锁mutex解锁。可见，从pthread_cond_wait开始执行到其调用线程被放入条件变量的等待队列之间的这段时间内，pthread_cond_signal和pthread_cond_broadcast等函数不会修改条件变量。换言之，pthread_cond_wait函数不会错过目标条件变量的任何变化^{[7]}。当pthread_cond_wait函数成功返回时，互斥锁mutex将再次被锁上。

摘自：《Linux高性能服务器编程》 — 游双
在豆瓣阅读书店查看：https://read.douban.com/ebook/15233070/
本作品由华章数媒授权豆瓣阅读中国大陆范围内电子版制作与发行。
© 版权所有，侵权必究。
 */
template <class T>
bool ThreadPool<T>::append(T &request)
{
      MutexGard MutexGard(queue_locker_);
      if (request_queue.size() == max_request_)
            return false;
      request_queue.push(request);
}

template <class T>
void ThreadPool<T>::run()
{
      T request;
      while (true)
      {
            {
                  MutexGard mutexGard(queue_locker_);
                  while (int ret = request_condition_.wait() && !shutdown_)
                  {
                        if (!request_queue_.empty())
                              break;
                        if (ret)
                              return;
                  }
                  if (shutdown_)
                        return;
                  request = request_queue_.front();
                  request_queue_.pop_front();
            }
            request->process();
      }
}

template <class T>
void *ThreadPool<T>::worker(void *arg)
{
      ThreadPool *pool = (ThreadPool *)arg;
      pool->run();
}

template <class T>
ThreadPool<T>::~ThreadPool()
{
      if (threads_)
            delete[] threads_;
      shutdown_ = true;
}
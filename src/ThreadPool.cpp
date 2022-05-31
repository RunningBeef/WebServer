//
// Created by yonggeng on 3/26/22.
//

#include "../include/ThreadPool.h"



int ThreadPool::MAX_TASK_SIZE = 1024;

int ThreadPool::MAX_THREAD_NUM = 10;


ThreadPool::ThreadPool(int threadNum, int maxTaskNum):thread_num_(threadNum), max_task_num_(maxTaskNum), shutdown_(
        false), condition_(mutex_) {/*注意用mutex_给condition_初始化*/
    
    if (thread_num_ < 1 || thread_num_ > MAX_THREAD_NUM) {
        std::cout << "set threadNum unfit " << std::endl;
        throw std::exception();
    }
    if (max_task_num_ < 1 || max_task_num_ > MAX_TASK_SIZE) {
        std::cout << "set maxTaskNum unfit " << std::endl;
        throw std::exception();
    }

    workThread_ = std::vector<pthread_t>(thread_num_, -1);
    for (auto &it: workThread_) {
        if (pthread_create(&it, nullptr, work, this)) {
            std::cout << "create thread unsuccess in file " << __FILE__ << " at line " << __LINE__ << std::endl;
            throw std::exception();
        }
        if (pthread_detach(it))/*https://blog.csdn.net/qq_33883085/article/details/89425933*/
        {
            std::cout << "error int file " << __FILE__ << " at line " << __LINE__ << std::endl;
            throw std::exception();
        }
    }
}


ThreadPool::~ThreadPool() {
    shutdown_ = true;
}


void * ThreadPool::work(void *arg) {
    ThreadPool *threadPoll = (ThreadPool *) arg;
    /* 每个线程都一直在运行这个函数，不断的处理http请求和响应 */
    threadPoll->run();
    return NULL;
}


/* mark1
 * pthread_cond_wait函数用于等待目标条件变量。
 * mutex参数是用于保护条件变量的互斥锁，以确保pthread_cond_wait操作的原子性。
 * 在调用pthread_cond_wait前，必须确保互斥锁mutex已经加锁，否则将导致不可预期的结果。
 * pthread_cond_wait函数执行时，首先把调用线程放入条件变量的等待队列中，然后将互斥锁mutex解锁。
 * 可见，从pthread_cond_wait开始执行到其调用线程被放入条件变量的等待队列之间的这段时间内，
 * pthread_cond_signal和pthread_cond_broadcast等函数不会修改条件变量。
 * 换言之，pthread_cond_wait函数不会错过目标条件变量的任何变化^{[7]}。
 * 当pthread_cond_wait函数成功返回时，互斥锁mutex将再次被锁上。

摘自：《Linux高性能服务器编程》 — 游双
在豆瓣阅读书店查看：https://read.douban.com/ebook/15233070/
本作品由华章数媒授权豆瓣阅读中国大陆范围内电子版制作与发行。
© 版权所有，侵权必究。*/

/* mark2
 * https://www.cnblogs.com/x_wukong/p/7909895.html
 * 1.pthread_cond_signal在多处理器上可能同时唤醒多个线程，
 * 当你只能让一个线程处理某个任务时，其它被唤醒的线程就需要继续 wait,while循环的意义就体现在这里了，
 * 而且规范要求pthread_cond_signal至少唤醒一个pthread_cond_wait上 的线程，
 * 其实有些实现为了简单在单处理器上也会唤醒多个线程.
   2.某些应用，如线程池，pthread_cond_broadcast唤醒全部线程，但我们通常只需要一部分线程去做执行任务，
   所以其它的线程需要继续wait.所以强烈推荐此处使用while循环.
   其实说白了很简单，就是pthread_cond_signal()也可能唤醒多个线程，而如果你同时只允许一个线程访问的话，
   就必须要使用while来进行条件判断，以保证临界区内只有一个线程在处理。
 * */

/* mark3 https://www.cnblogs.com/diyingyun/archive/2011/11/25/2263164.html
 * pthread_cond_wait() 所做的第一件事就是同时对互斥对象解锁（于是其它线程可以修改已链接列表 我理解应该是指条件变量），并等待条件 mycond 发生（这样当 pthread_cond_wait() 接收到另一个线程的“信号”时，它将苏醒）。现在互斥对象已被解锁，其它线程可以访问和修改已链接列表，可能还会添加项。 【要求解锁并阻塞是一个原子操作】
  此时，pthread_cond_wait() 调用还未返回。
  对互斥对象解锁会立即发生，但等待条件 mycond 通常是一个阻塞操作，这意味着线程将睡眠，
  在它苏醒之前不会消耗 CPU 周期。这正是我们期待发生的情况。线程将一直睡眠，直到特定条件发生，
  在这期间不会发生任何浪费 CPU 时间的繁忙查询。
  从线程的角度来看，它只是在等待 pthread_cond_wait() 调用返回。
  现在继续说明，假设另一个线程（称作“2 号线程”）锁定了 mymutex 并对已链接列表添加了一项。
  在对互斥对象解锁之后，2 号线程会立即调用函数 pthread_cond_broadcast(&mycond)。
  此操作之后，2 号线程将使所有等待 mycond 条件变量的线程立即苏醒。
  这意味着第一个线程（仍处于 pthread_cond_wait() 调用中）现在将苏醒。

  现在，看一下第一个线程发生了什么。
  您可能会认为在 2 号线程调用 pthread_cond_broadcast(&mymutex) 之后，
  1 号线程的 pthread_cond_wait() 会立即返回。
  不是那样！实际上，pthread_cond_wait() 将执行最后一个操作：重新锁定 mymutex。
  一旦 pthread_cond_wait() 锁定了互斥对象，那么它将返回并允许 1 号线程继续执行。
  那时，它可以马上检查列表（相当于下面的run函数，检查任务队列是否为空），查看它所感兴趣的更改。
 * */

void ThreadPool::run() {
    Task task;
    while (true) {

        {
            MutexGuard mutexGuard(mutex_);/*mark1*/
            while (task_queue_.empty() && !shutdown_) {/*mark2*/
                condition_.wait();/*mark3*/
            }
            if (shutdown_) {/*线程池结束标记*/
                return ;
            }
            task = task_queue_.front();
            task_queue_.pop();
        }
        task.function(task.arg);/*先解锁后执行任务，执行任务过程中占用锁是没有意义的*/
    }
}


void ThreadPool::appendTask(Task task) {
    MutexGuard mutexGuard(mutex_);
    {
        if (task_queue_.size() == max_task_num_) {
            std::cout << "to more task undo ! error in threadPool" << std::endl;
        }
        task_queue_.push(task);
        condition_.notify();/*记得唤醒工作线程*/
    }
}
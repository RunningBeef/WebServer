#ifndef TIMER_H
#define TIMER_H

#include "../locker/Mutex.h"
#include <queue>
#include <memory>
#include <vector>
#include <sys/time.h>
#include <functional>
#include <iostream>
#define MAX_TRICK 500
#define DEFAULT_KEEP_ALIVE_TIME 60
#define DEFAULT_KEEP_ALIVE_MAX 100000
/*
## 长连接实现
①简单版本
      移除超时连接（移除定时器执行定时任务）和 新连接或者旧连接新请求（断开旧定时器，加入新定时器）都在主线程实现
      缺点：只考虑默认长连接的HTTP协议，也不考虑处理过程中发现不合法HTTP请求时直接断开请求
      优点：无锁性能好
②复杂版本
      移除超时连接在主线程，新连接或就连接请求到来，断开定时器的关联也在主线程，但是否加入定时器在子线程
      所以对于trick 和 append 需要加锁

      https://blog.51cto.com/u_15688254/5392429
      Http1.0除非客户端使用了Connection: keep-alive(并且服务端响应报文也要发送该头部字段) 否则就是短连接
      Http1.1默认长连接，请求头不用指定Connection: keep-alive，响应头需要指定
      需要检查Connection: close，如果该字段被设置（两端都有可能）则处理后断开连接

      不难看出这些操作需要处理Http请求时来做，而处理HTTP请求是在工作线程中实现的，所以还是需要用加锁操作
      keep-alive 设置超时时间，和期间最大请求数
      响应报文设置 keep-alive   https://developer.mozilla.org/en-US/docs/Web/HTTP/Headers/Keep-Alive
 */

/*
      如何设计一个通用的定时器（通过回调接口）
      要求：①回调接口参数可能有多个。②要让实现不同定时任务的定时器可以被同一个定时器管理类管理（不能用类模板）。
      方案：
      ①可以考虑使用如下结构cbTask（②
      如果回调接口有多个参数，则可以使用bind方法绑定（①
      ②可以使用父类指针或引用指向子类对象，通过调用虚函数来表示回调接口（②
      但是这样还是有可能出现回调任务参数不同的情况，
      只能让对应参数直接包括在类对象中①，这样不利于原先定义好的类，各个类互相包含也会提高bug概率，
      而且这个参数可能只是回调时用一下，没必要一直保存在类中
 */
struct cbTask
{
      cbTask(std::function<void(std::shared_ptr<void>)> cb_func, std::shared_ptr<void> arg)
          : cb_func_(cb_func), arg_(arg)
      {
      }
      std::function<void(std::shared_ptr<void>)> cb_func_;
      std::shared_ptr<void> arg_;
};

class TimerNode
{
public:
      TimerNode(std::shared_ptr<cbTask> cb_task_ptr, size_t interval = DEFAULT_KEEP_ALIVE_TIME);
      ~TimerNode();
      void cancelTask();
      size_t getOutTime();
      void setOutTime(size_t new_time);
private:
      //定时器通用化，可以使用模板类，如果参数个数不确定，也可以使用可变参数模板
      std::shared_ptr<cbTask> cb_task_ptr_;
      size_t out_time_;
      // bool is_break;//旧连接再次请求时标记为断开，可以直接对http_data reset 后判空就行
};

//Comparision object
class TimerCmp
{
      //lhs left hand side
      bool operator()(const std::shared_ptr<TimerNode> &lhs, const std::shared_ptr<TimerNode> &rhs)
      {
            return lhs->getOutTime() > rhs->getOutTime();
      }
};
class TimerManager
{
public:
      void append(std::shared_ptr<TimerNode>);
      void trick();
private:
      std::priority_queue<std::shared_ptr<TimerNode>,
      std::vector<std::shared_ptr<TimerNode>>,TimerCmp> timer_queue;
      Mutex mutex_;//导致该类不可拷贝
      int max_trick_ = MAX_TRICK; //设置trick 每次最多可以处理的定时任务
      static size_t cur_sec_;
};

#endif
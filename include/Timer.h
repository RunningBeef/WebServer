#ifndef TIMER_H
#define TIMER_H
#include "HttpData.h"
#include "Mutex.h"
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
template<class T>
class TimerNode
{
public:
      TimerNode(std::shared_ptr<HttpData> http_data_ptr);
      ~TimerNode();
private:
      //定时器通用化，可以使用模板类，如果参数个数不确定，也可以使用可变参数模板
      std::shared_ptr<void> task;
      size_t out_time_;
      //bool is_break;//旧连接再次请求时标记为断开，可以直接对http_data reset 后判空就行
};

template<class T>
class TimerManager
{
public:
      void append(std::shared_ptr<TimerNode<T>>);
      void trick();
private:
      int max_trick_;//设置trick 每次最多可以处理的定时任务
      Mutex mutex_;
};


#endif
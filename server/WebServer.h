#ifndef WEBSERVER_H
#define WEBSERVER_H
#include "../http/HttpData.h"
#include "../threadPool/ThreadPool.h"
#include "../timer/Timer.h"

class WebServer : public NonCopyable
{
public:
      void process(std::shared_ptr<HttpData>);
private:
      ThreadPool<WebServer> * m_thread_pool_ptr_;
      TimerManager m_timer_manger_;
};
#endif
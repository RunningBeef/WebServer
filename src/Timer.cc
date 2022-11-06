#include "../include/Timer.h"

TimerNode::TimerNode(std::shared_ptr<cbTask> cb_task_ptr, size_t interval = DEFAULT_KEEP_ALIVE_TIME)
    : cb_task_ptr_(cb_task_ptr)
{
      struct timeval cur;
      gettimeofday(&cur, NULL);
      out_time_ = cur.tv_sec + interval;
}

TimerNode::~TimerNode()
{
      if (cb_task_ptr_)
      {
            cb_task_ptr_->cb_func_(cb_task_ptr_->arg_);
      }
}

void TimerNode::cancelTask()
{
      //重置为空
      cb_task_ptr_.reset();
}

void TimerNode::setOutTime(size_t new_time)
{
      out_time_ = new_time;
}

size_t TimerNode::getOutTime()
{
      return out_time_;
}
void TimerManager::append(std::shared_ptr<TimerNode> timer_ptr)
{
      {
            MutexGard lock(mutex_);
            timer_queue.push(timer_ptr);
      }
}

void TimerManager::trick()
{
      struct timeval cur_time;
      gettimeofday(&cur_time, NULL);
      cur_sec_ = cur_time.tv_sec;
      {
            MutexGard lock(mutex_);
            int ct = 0;
            while(!timer_queue.empty() && ++ct < max_trick_
            && timer_queue.top()->getOutTime() <= cur_sec_)
            {
                  timer_queue.pop();
            }

            if(ct == max_trick_)
            {
                  std::cout << "too much task out time" << std::endl;
            }
      }
}

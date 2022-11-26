/*
 * @Author: RunningBeef 2723772192@qq.com
 * @Date: 2022-11-08 12:12:29
 * @LastEditors: RunningBeef 2723772192@qq.com
 * @LastEditTime: 2022-11-26 12:29:18
 * @FilePath: /lighthouse/WebServer/locker/Condition.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#ifndef CONDITION_H
#define CONDITION_H
#include "NonCopyable.h"
#include "Mutex.h"
#include <pthread.h>
#include <iostream>
class Condition : public NonCopyable
{
public:
      Condition(Mutex &mutex);
      ~Condition();
      void signal();
      void broadCast();
      int wait();

private:
      Mutex &mutex_reference_;
      pthread_cond_t cond_;
};
#endif
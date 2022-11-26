/*
 * @Author: RunningBeef 2723772192@qq.com
 * @Date: 2022-11-08 12:12:29
 * @LastEditors: RunningBeef 2723772192@qq.com
 * @LastEditTime: 2022-11-26 16:02:10
 * @FilePath: /lighthouse/WebServer/locker/Condition.cc
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include "Condition.h"

Condition::Condition(Mutex &mutex) : mutex_reference_(mutex)
{
      pthread_cond_init(&cond_, NULL);
#ifdef DEBUG
      std::cout << "COndition construct success" << std::endl;
#endif
}

Condition::~Condition()
{
      pthread_cond_destroy(&cond_);
#ifdef DEBUG
      std::cout << "Condition destroy success" << std::endl;
#endif
}
void Condition::broadCast()
{
      pthread_cond_broadcast(&cond_);
}
void Condition::signal()
{
      pthread_cond_signal(&cond_);
}
int Condition::wait()
{
      return pthread_cond_wait(&cond_, &mutex_reference_.getMutex());
}

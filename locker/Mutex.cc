/*
 * @Author: RunningBeef 2723772192@qq.com
 * @Date: 2022-11-08 12:12:29
 * @LastEditors: RunningBeef 2723772192@qq.com
 * @LastEditTime: 2022-11-26 12:32:23
 * @FilePath: /lighthouse/WebServer/locker/Mutex.cc
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include "Mutex.h"
Mutex::Mutex()
{
      pthread_mutex_init(&mutex_, NULL);
#ifdef DEBUG
      std::cout << "Mutex construct success" << std::endl;
#endif
}
Mutex::~Mutex()
{
      pthread_mutex_destroy(&mutex_);
#ifdef DEBUG
      std::cout << "Mutex destroy success" << std::endl;
#endif
}
void Mutex::lock()
{
      pthread_mutex_lock(&mutex_);
}
int Mutex::tryLock()
{
      // 成功时返回 0 ，失败返回错误
      return pthread_mutex_trylock(&mutex_);
}
void Mutex::unlock()
{
      pthread_mutex_unlock(&mutex_);
}
pthread_mutex_t &Mutex::getMutex()
{
      return mutex_;
}
MutexGard::MutexGard(Mutex &mutex) : mutex_reference_(mutex)
{
      mutex_reference_.lock();
#ifdef DEBUG
      std::cout << "MutexGard construct success" << std::endl;
#endif
}

MutexGard::~MutexGard()
{
      mutex_reference_.unlock();
#ifdef DEBUG
      std::cout << "MutexGard destroy success" << std::endl;
#endif
}
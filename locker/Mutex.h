/*
 * @Author: RunningBeef 2723772192@qq.com
 * @Date: 2022-11-08 12:12:29
 * @LastEditors: RunningBeef 2723772192@qq.com
 * @LastEditTime: 2022-11-26 12:29:38
 * @FilePath: /lighthouse/WebServer/locker/Mutex.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#ifndef MUTEX_H
#define MUTEX_H
#include "NonCopyable.h"
#include <pthread.h>
#include <iostream>
class Mutex : public NonCopyable
{
public:
      Mutex();
      ~Mutex();
      void lock();
      void unlock();
      int tryLock();
      pthread_mutex_t &getMutex();

private:
      pthread_mutex_t mutex_;
};

/*
避免单参数构造函数隐式调用带来的风险，以及提高代码可读性，使用explicit声明为显示构造
https://cloud.tencent.com/developer/article/1347881#:~:text=%E5%9C%A8%E6%B2%A1%E6%9C%89%E5%90%88%E9%80%82%E7%90%86%E7%94%B1%E5%BF%85,%E5%87%BD%E6%95%B0%E7%9A%84%E9%9A%90%E5%BC%8F%E8%B0%83%E7%94%A8%E3%80%82
*/
class MutexGard
{
public:
      explicit MutexGard(Mutex &mutex);
      ~MutexGard();

private:
      Mutex &mutex_reference_;
};
#endif
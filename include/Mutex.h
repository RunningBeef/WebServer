#ifndef MUTEX_H
#define MUTEX_H
#include "NonCopyable.h"
#include <pthread.h>

class Mutex: public NonCopyable{
public:
      Mutex();
      ~Mutex();
      void lock();
      void unlock();
      int tryLock();
private:
      pthread_mutex_t mutex_;
};

class MutexGard{
public:
      MutexGard(Mutex & mutex);
      ~MutexGard();
private:
      Mutex & mutex_reference_;
};
#endif
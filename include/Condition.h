#ifndef CONDITION_H
#define CONDITION_H
#include "NonCopyable.h";
#include "Mutex.h"
#include <pthread.h>


class Condition : public NonCopyable{
public:
      Condition(Mutex & mutex);
      ~Condition();
      void signal();
      void broadCast();
      int wait();
private:
      Mutex & mutex_reference_;
      pthread_cond_t cond_;
};
#endif
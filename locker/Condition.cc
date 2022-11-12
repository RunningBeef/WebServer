#include "Condition.h"

Condition::Condition(Mutex &mutex) : mutex_reference_(mutex)
{
      pthread_cond_init(&cond_, NULL);
}

Condition::~Condition()
{
      pthread_cond_destroy(&cond_);
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

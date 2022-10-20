#include "../include/Mutex.h"

Mutex::Mutex(){
      pthread_mutex_init(&mutex_,NULL);
}
Mutex::~Mutex(){
      pthread_mutex_destroy(&mutex_);
}
void Mutex::lock(){
      pthread_mutex_lock(&mutex_);
}
int Mutex::tryLock(){
      // 成功时返回 0 ，失败返回错误
      return pthread_mutex_trylock(&mutex_);
}
void Mutex::unlock(){
      pthread_mutex_unlock(&mutex_);
}
pthread_mutex_t & Mutex::getMutex(){
      return mutex_;
}
MutexGard::MutexGard(Mutex & mutex):mutex_reference_(mutex){
      mutex_reference_.lock();
}

MutexGard::~MutexGard(){
      mutex_reference_.unlock();
}
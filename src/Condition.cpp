//
// Created by yonggeng on 3/23/22.
//
#include "../include/Condition.h"

Condition::Condition(Mutex & mutex): mutex_(mutex){
    pthread_cond_init(&cond_,NULL);
}

Condition::~Condition(){
    pthread_cond_destroy(&cond_);
}
void Condition::notify(){
    pthread_cond_signal(&cond_);
}
void Condition::notifyAll() {
    pthread_cond_broadcast(&cond_);
}
void Condition::wait(){
    pthread_cond_wait(&cond_,mutex_.getMutex());
}
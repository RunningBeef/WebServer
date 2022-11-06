#include "../include/Timer.h"

template<class T>
TimerNode<T>::TimerNode(std::shared_ptr<T> task_ptr)
:task_ptr_(task_ptr)
{

}
template<class T>
TimerNode<T>::~TimerNode()
{
      
}
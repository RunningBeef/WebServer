// #pragma once

/* 功能
1.扩容 和 压缩
2.线程安全
3.支持各种类型的分配
4.清除归还的内存的内容
 */
#include "Mutex.h"
#include <memory>
#include <iostream>

template <typename T>
class MemoryPoll
{
public:
    MemoryPoll()
    {
        used = 0;
        sz = 32;
        arr = new (T *)[sz];
        for (int i = 0; i < sz; ++i)
        {
            arr[i] = new T;
        }
    }
    ~MemoryPoll()
    {
        /* 这块可能有问题 */
        if (arr != nullptr)
        {
            delete arr[];
        }
    }

    T *distribution();
    {
        /* 保证线程安全 */
        MutexGuard(mutex_);

        /* 判断是否扩容 */
        if ((used << 1) > sz)
        {
            resize(sz << 1);
        }
        return arr[used++];
    }

    /* 这块参数命名可能也有问题 */
    void recovery(T * & element)
    {
        MutexGuard(mutex_);
        /* 判断是否压缩 */
        if((used >> 4) < sz)
        {
            resize(sz >> 2);
        }

        if (--used < 0)
        {
            std::cout << "to more recovery in file " << __FILE__ << " at line " << __LINE__ << std::endl;

            exit(1);
        }
        arr[used] = element;
    
    }

    void resize(int newSz)
    {
        T ** arr2 = new (T * )[newSz];
        
        /* 直接利用之前有的 */
        for(int i = used; i < min(sz,newSz);++i)
        {
            arr2[i] = arr[i];
        }

        /* 当扩容时再申请不够用的 */   
        for(int i = sz; i < newSz; ++i)
        {
            arr2[i] = new T;
        }

        /* 当压缩时回收多余的 */
        for(int i = newSz; i < sz; ++i)
        {
            delete arr[i];
        }
        arr = arr2;
        size = newSz;
    }

private:
    int used, sz;
    T **arr;
    Mutex mutex_;
};
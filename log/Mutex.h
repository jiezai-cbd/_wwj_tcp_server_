#ifndef _MUTEX_H_
#define _MUTEX_H_
#include "Thread.h"
#include <pthread.h>
namespace wwj
{
class MutexLock
{
public:
    MutexLock():holder_(0)
    {
        pthread_mutex_init(&mutex_,NULL);
    }
    ~MutexLock()
    {
        pthread_mutex_destroy(&mutex_);
    }
    void lock()
    {
        pthread_mutex_lock(&mutex_);
        holder_=CurrentThread::tid();
    }

    void unlock()
    {
        holder_=0;
        pthread_mutex_unlock(&mutex_);
    }

    pthread_mutex_t* getPthreadMutex() 
    {
        return &mutex_;
    }

private:
    pthread_mutex_t mutex_;
    pid_t holder_;
};


class MutexLockGuard
{
public:
    MutexLockGuard(MutexLock& mutex):mutex_(mutex)
    {
        mutex_.lock();
    }
    ~MutexLockGuard()
    {
        mutex_.unlock();
    }
private:
    MutexLock& mutex_;

};
}

#endif
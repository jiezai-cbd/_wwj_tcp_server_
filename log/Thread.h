#ifndef _THREAD_H_
#define _THREAD_H_
#include <pthread.h>
#include <functional>
#include <atomic>
namespace wwj
{
class Thread
{
public:
    typedef std::function<void ()> ThreadFunc;
    Thread(const ThreadFunc&, const std::string& name = std::string());
    ~Thread();
    void start();
    void join();

    bool started() const 
    { 
        return started_; 
    }
    pid_t tid() const 
    { 
        return tid_; 
    }
    const std::string& name() const 
    { 
        return name_; 
    }

    static int numCreated() 
    { 
        return numCreated_.load(std::memory_order_relaxed); 
    }


private:

    bool started_;
    bool joined_;
    pthread_t  pthreadId_;
    pid_t tid_;
    ThreadFunc  func_;
    std::string name_;

    static std::atomic<int32_t> numCreated_;

};

namespace CurrentThread
{
    pid_t tid();
    const char* name();
    bool isMainThread();
}
}

#endif
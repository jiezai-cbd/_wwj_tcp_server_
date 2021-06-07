#ifndef _EVENTLOOPTHREADPOOL_H_
#define _EVENTLOOPTHREADPOOL_H_

#include "Condition.h"
#include "Mutex.h"
#include "Thread.h"
#include <vector>
#include <functional>
#include <memory>

namespace wwj
{

class EventLoop;
class EventLoopThread;

class EventLoopThreadPool 
{
public:
    EventLoopThreadPool(EventLoop* baseLoop);
    ~EventLoopThreadPool();
    void setThreadNum(int numThreads) 
    { 
        numThreads_ = numThreads; 
    }
    void start();
    EventLoop* getNextLoop();

private:
    EventLoop* baseLoop_;
    bool started_;
    int numThreads_;
    int next_;  
    std::vector<std::unique_ptr<EventLoopThread>> threads_;
    std::vector<EventLoop*> loops_;
};

}

#endif 

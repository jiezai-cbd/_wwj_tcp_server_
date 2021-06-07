#ifndef _EVENTLOOPTHREAD_H_
#define _EVENTLOOPTHREAD_H_
#include "Condition.h"
#include "Mutex.h"
#include "Thread.h"


namespace wwj
{

class EventLoop;

class EventLoopThread
{
public:
  EventLoopThread();
  ~EventLoopThread();
  EventLoop* startLoop();

private:
  void threadFunc();

  EventLoop* loop_;
  bool exiting_;
  Thread thread_;
  MutexLock mutex_;
  Condition cond_;
};

}

#endif
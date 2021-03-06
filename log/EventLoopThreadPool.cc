#include "EventLoopThreadPool.h"
#include "EventLoop.h"
#include "EventLoopThread.h"

#include <functional>

using namespace wwj;

EventLoopThreadPool::EventLoopThreadPool(EventLoop* baseLoop)
  : baseLoop_(baseLoop),
    started_(false),
    numThreads_(0),
    next_(0)
{
}

EventLoopThreadPool::~EventLoopThreadPool()
{

}

void EventLoopThreadPool::start()
{
  started_ = true;
  for (int i = 0; i < numThreads_; ++i)
  {
    EventLoopThread* t = new EventLoopThread;
    threads_.push_back(std::unique_ptr<EventLoopThread>(t));
    loops_.push_back(t->startLoop());
  }
}

EventLoop* EventLoopThreadPool::getNextLoop()
{
  EventLoop* loop = baseLoop_;
  if (!loops_.empty())
  {
    loop = loops_[next_];
    ++next_;
    if (static_cast<size_t>(next_) >= loops_.size())
    {
      next_ = 0;
    }
  }
  return loop;
}


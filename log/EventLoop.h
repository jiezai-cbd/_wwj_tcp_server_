#ifndef _EVENTLOOP_H_
#define _EVENTLOOP_H_
#include <vector>
#include "Timestamp.h"
#include <memory>
#include <unistd.h>
#include <functional>
#include "Mutex.h"
#include "Thread.h"
#include "TimerId.h"
#include "Callbacks.h"

namespace wwj
{
class Channel;
class Poller;
class TimerQueue;
class EventLoop
{
public:
    typedef std::function<void()> Functor;
    EventLoop();
    ~EventLoop();
    void updateChannel(Channel* channel);
    void removeChannel(Channel* channel);
    void quit();
    void loop();
    void wakeup();
    void queueInLoop(const Functor& cb);

    TimerId runAt(const Timestamp& time, const TimerCallback& cb);
    TimerId runAfter(double delay, const TimerCallback& cb);
    TimerId runEvery(double interval, const TimerCallback& cb);
    bool isInLoopThread() const { return threadId_ == CurrentThread::tid(); }
    void runInLoop(const Functor& cb);
  void cancel(TimerId timerId);
private:
    void handleRead();
    void doPendingFunctors();
    bool looping_;
    bool quit_;
    const pid_t threadId_;
    std::unique_ptr<Poller> poller_;
    std::unique_ptr<TimerQueue> timerQueue_;
    
    typedef std::vector<Channel*> ChannelList;
    ChannelList activeChannels_;
    Timestamp pollReturnTime_;
    int wakeupFd_;
    std::unique_ptr<Channel> wakeupChannel_;
    bool callingPendingFunctors_;
    std::vector<Functor> pendingFunctors_;
    MutexLock mutex_;
};



}

#endif
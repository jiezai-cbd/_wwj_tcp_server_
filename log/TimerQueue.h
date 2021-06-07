#ifndef _TIMERQUEUE_H_
#define _TIMERQUEUE_H_
#include "Channel.h"
#include "Timestamp.h"
#include <vector>
#include <set>
#include "Callbacks.h"
namespace wwj
{
class EventLoop;
class Timer;
class TimerId;
class TimerQueue
{
public:
    TimerQueue(EventLoop* loop);
    ~TimerQueue();
    TimerId addTimer(const TimerCallback& cb,Timestamp when,double interval);
    void cancel(TimerId timerId);
    

private:
    typedef std::pair<Timestamp,Timer*> Entry;
    typedef std::set<Entry> TimerList;
    typedef std::pair<Timer*,int64_t> ActiveTimer;
    typedef std::set<ActiveTimer> ActiveTimerSet;
    void reset(const std::vector<Entry>& expired,Timestamp now);
    void addTimerInLoop(Timer* timer);
    void cancelInLoop(TimerId timerId);
    void handleRead();
    std::vector<Entry> getExpired(Timestamp now);
    bool insert(Timer* timer);
    EventLoop* loop_;
    const int timerfd_;
    bool callingExpiredTimers_;
    Channel timerfdChannel_;
    TimerList timers_;
    
    ActiveTimerSet activeTimers_;
    ActiveTimerSet cancelingTimers_;
};   
}

#endif
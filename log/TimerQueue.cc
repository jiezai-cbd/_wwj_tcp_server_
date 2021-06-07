#include "TimerQueue.h"

#include "Logging.h"
#include "EventLoop.h"
#include <sys/timerfd.h>
#include <functional>
#include "Timer.h"
#include "TimerId.h"
#include <algorithm>
namespace wwj
{
namespace detail
{
int createTimerfd()
{
    int timerfd = ::timerfd_create(CLOCK_MONOTONIC,
                                 TFD_NONBLOCK | TFD_CLOEXEC);
    if(timerfd<0)
    {
        LOG_SYSFATAL<<"Failed in timerfd_create";
    }
    return timerfd;
}
void readTimerfd(int timerfd,Timestamp now)
{
    uint64_t howmany;
    ssize_t n = ::read(timerfd,&howmany,sizeof howmany);
    LOG_TRACE<<"TimerQueue::handleRead() "<< static_cast<int>(howmany)<<" at "<<now.toString().c_str();
    //if(n!=sizeof howmany)
}

struct timespec howMuchTimeFromNow(Timestamp when)
{
  int64_t microseconds = when.microSecondsSinceEpoch()
                         - Timestamp::now().microSecondsSinceEpoch();
  if (microseconds < 100)
  {
    microseconds = 100;
  }
  struct timespec ts;
  ts.tv_sec = static_cast<time_t>(
      microseconds / Timestamp::kMicroSecondsPerSecond);
  ts.tv_nsec = static_cast<long>(
      (microseconds % Timestamp::kMicroSecondsPerSecond) * 1000);
  return ts;
}

void resetTimerfd(int timerfd, Timestamp expiration)
{
    struct itimerspec newValue;
    struct itimerspec oldValue;
    bzero(&newValue, sizeof newValue);
    bzero(&oldValue, sizeof oldValue);
    newValue.it_value = howMuchTimeFromNow(expiration);
    int ret = ::timerfd_settime(timerfd, 0, &newValue, &oldValue);
    if (ret)
    {
        LOG_SYSERR << "timerfd_settime()";
    }
}

} 
}

using namespace wwj;
using namespace wwj::detail;
TimerQueue::TimerQueue(EventLoop* loop)
:loop_(loop),
timerfd_(createTimerfd()),
timers_(),
timerfdChannel_(loop,timerfd_),
callingExpiredTimers_(false)
{
    timerfdChannel_.setReadCallback(std::bind(&TimerQueue::handleRead,this));
    LOG_TRACE<<"TimerQueue::TimerQueue() before enableReading()";
    timerfdChannel_.enableReading();
    LOG_TRACE<<"TimerQueue::TimerQueue() after enableReading()";
}
TimerQueue::~TimerQueue()
{
    ::close(timerfd_);
    for(TimerList::iterator it=timers_.begin();it!=timers_.end();++it)
    {
        delete it->second;
    }
}

void TimerQueue::handleRead()
{
    Timestamp now(Timestamp::now());
    readTimerfd(timerfd_,now);
    std::vector<Entry> expired=getExpired(now);
    callingExpiredTimers_=true;

    for(std::vector<Entry>::iterator it=expired.begin();it!=expired.end();++it)
    {
        it->second->run();
    }
    callingExpiredTimers_=false;
    reset(expired,now);
}

std::vector<TimerQueue::Entry> TimerQueue::getExpired(Timestamp now)
{
    std::vector<Entry> expired;
    Entry sentry = std::make_pair(now,reinterpret_cast<Timer*>(UINTPTR_MAX));
    TimerList::iterator it = timers_.lower_bound(sentry);
    std::copy(timers_.begin(), it, back_inserter(expired));
    timers_.erase(timers_.begin(), it);

    for(const Entry entry:expired)
    {
        ActiveTimer timer(entry.second,entry.second->sequence());
        activeTimers_.erase(timer);
    }
    return expired;

}

void TimerQueue::reset(const std::vector<Entry>& expired,Timestamp now)
{
    Timestamp nextExpire;
    for (std::vector<Entry>::const_iterator it = expired.begin();
      it != expired.end(); ++it)
    {
        ActiveTimer timer(it->second, it->second->sequence());
        if (it->second->repeat()&& cancelingTimers_.find(timer) == cancelingTimers_.end())
        {
            it->second->restart(now);
            insert(it->second);
        }
        else
        {
            delete it->second;
        }

    }

    if (!timers_.empty())
    {
        nextExpire = timers_.begin()->second->expiration();
    }

    if (nextExpire.valid())
    {
        resetTimerfd(timerfd_, nextExpire);
    }



}

bool TimerQueue::insert(Timer* timer)
{
    bool earliestChanged = false;
    Timestamp when = timer->expiration();
    TimerList::iterator it = timers_.begin();
    //set排序
    if (it == timers_.end() || when < it->first)
    {
        earliestChanged = true;
    }
    timers_.insert(Entry(when, timer));
    activeTimers_.insert(ActiveTimer(timer, timer->sequence()));
    return earliestChanged;
}

TimerId TimerQueue::addTimer(const TimerCallback& cb,Timestamp when,double interval)
{
  Timer* timer = new Timer(cb, when, interval);
  loop_->runInLoop(std::bind(&TimerQueue::addTimerInLoop, this, timer));
  return TimerId(timer, timer->sequence());
}

void TimerQueue::cancel(TimerId timerId)
{
  loop_->runInLoop(std::bind(&TimerQueue::cancelInLoop, this, timerId));
}

void TimerQueue::addTimerInLoop(Timer* timer)
{
  bool earliestChanged = insert(timer);

  if (earliestChanged)
  {
    resetTimerfd(timerfd_, timer->expiration());
  }
}

void TimerQueue::cancelInLoop(TimerId timerId)
{
  ActiveTimer timer(timerId.timer_, timerId.sequence_);
  ActiveTimerSet::iterator it = activeTimers_.find(timer);
  //不对啊，本来就是顺序执行的(在一个线程内)
  //我懂了，嵌套调用，在queueinloop中调用queueinloop，此时就有可能发生这种情况。

  //还在激活的时间队列中，即此时还没有超时，或者getexpired还没有将其取出来，要取消只需要erase即可
  if (it != activeTimers_.end())
  {
    timers_.erase(Entry(it->first->expiration(), it->first));
    delete it->first;
    activeTimers_.erase(it);
  }
  //不在激活的时间队列中，正在调用超时回调，此时已经getexpired取了出来，将其设置为已取消的，后面的reset会删除。
  else if (callingExpiredTimers_)
  {
    cancelingTimers_.insert(timer);
  }
  //不在激活队列中，且没有调用超时回调
}



#include "EventLoop.h"
#include "Poller.h"
#include "Logging.h"
#include "Channel.h"
#include "TimerQueue.h"
#include <signal.h>
#include <sys/eventfd.h>

using namespace wwj;

const int kPollTimeMs=10000;
static int createEventfd()
{
    int evtfd = ::eventfd(0, EFD_NONBLOCK | EFD_CLOEXEC);
    if (evtfd < 0)
    {
        LOG_SYSERR << "Failed in eventfd";
        abort();
    }
    return evtfd;
}


class IgnoreSigPipe
{
 public:
  IgnoreSigPipe()
  {
    ::signal(SIGPIPE, SIG_IGN);
  }
};

IgnoreSigPipe initObj;


EventLoop::EventLoop()
:looping_(false),
quit_(false),
threadId_(CurrentThread::tid()),
poller_(new Poller(this)),
callingPendingFunctors_(false),
timerQueue_(new TimerQueue(this)),
wakeupFd_(createEventfd()),
wakeupChannel_(new Channel(this,wakeupFd_))
{
    LOG_TRACE << "EventLoop created " ;
    wakeupChannel_->setReadCallback(std::bind(&EventLoop::handleRead, this));
    wakeupChannel_->enableReading();
}

/*
EventLoop::EventLoop()
:looping_(false),
quit_(false),
threadId_(CurrentThread::tid()),
poller_(new Poller(this)),
callingPendingFunctors_(false),

wakeupFd_(createEventfd()),
wakeupChannel_(new Channel(this,wakeupFd_))
{
    LOG_TRACE << "EventLoop created " ;
    wakeupChannel_->setReadCallback(std::bind(&EventLoop::handleRead, this));
    wakeupChannel_->enableReading();
}
*/
EventLoop::~EventLoop()
{
    //::close(wakeupFd_);
}

void EventLoop::quit()
{
    quit_=true;
    if (!isInLoopThread())
    {
        wakeup();
    }
}

void EventLoop::updateChannel(Channel* channel)
{
    LOG_TRACE<<"EventLoop::updateChannel   before poller_->updateChannel(channel);";
    poller_->updateChannel(channel);
    LOG_TRACE<<"EventLoop::updateChannel   after poller_->updateChannel(channel);";
}
void EventLoop::loop()
{
    looping_=true;
    quit_=false;
    while(!quit_)
    {
        activeChannels_.clear();//who delete channel?
        pollReturnTime_=poller_->poll(kPollTimeMs,&activeChannels_);
        for(ChannelList::iterator it = activeChannels_.begin();it!=activeChannels_.end();++it)
        {
            (*it)->handleEvent(pollReturnTime_);
        }
        doPendingFunctors();
    }
    LOG_TRACE<<"EventLoop "<<"stop looping";
    looping_=false;
}

void EventLoop::doPendingFunctors()
{
    std::vector<Functor> functors;
    callingPendingFunctors_=true;
    {
        MutexLockGuard lock(mutex_);
        functors.swap(pendingFunctors_);
    }
    for(size_t i=0;i<functors.size();++i)
    {
        functors[i]();
    }

    callingPendingFunctors_=false;
}

TimerId EventLoop::runAt(const Timestamp& time, const TimerCallback& cb)
{
  return timerQueue_->addTimer(cb, time, 0.0);
}

TimerId EventLoop::runAfter(double delay, const TimerCallback& cb)
{
  Timestamp time(addTime(Timestamp::now(), delay));
  return runAt(time, cb);
}

TimerId EventLoop::runEvery(double interval, const TimerCallback& cb)
{
  Timestamp time(addTime(Timestamp::now(), interval));
  return timerQueue_->addTimer(cb, time, interval);
}

void EventLoop::cancel(TimerId timerId)
{
  return timerQueue_->cancel(timerId);
}

void EventLoop::runInLoop(const Functor& cb)
{
  if (isInLoopThread())
  {
    cb();
  }
  else
  {
    queueInLoop(cb);
  }
}

void EventLoop::queueInLoop(const Functor& cb)
{
  {
  MutexLockGuard lock(mutex_);
  pendingFunctors_.push_back(cb);
  }

  if (!isInLoopThread() || callingPendingFunctors_)
  {
    wakeup();
  }
}

void EventLoop::handleRead()
{
  uint64_t one = 1;
  ssize_t n = ::read(wakeupFd_, &one, sizeof one);
}

void EventLoop::wakeup()
{
  uint64_t one = 1;
  ssize_t n = ::write(wakeupFd_, &one, sizeof one);
  
}

void EventLoop::removeChannel(Channel* channel)
{
  poller_->removeChannel(channel);
}
#ifndef _TIMER_H_
#define _TIMER_H_
#include "Callbacks.h"
#include "Timestamp.h"
#include <atomic>
namespace wwj
{
class Timer
{
public:
    Timer(const TimerCallback cb,Timestamp when,double interval)
    :callback_(cb),expiration_(when),interval_(interval),repeat_(interval>0.0),sequence_(++s_numCreated_)
    {

    }
    void run() const
    {
        callback_();
    }
    Timestamp expiration() const  { return expiration_; }
    int64_t sequence() const {return sequence_;}
    bool repeat() const { return repeat_; }
    void restart(Timestamp now);
private:
    const double interval_;
    const TimerCallback callback_;
    const int64_t sequence_;
    const bool repeat_;
    Timestamp expiration_;
    static std::atomic<int64_t> s_numCreated_;

};


}

#endif
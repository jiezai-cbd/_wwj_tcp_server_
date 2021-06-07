#ifndef _CHANNEL_H_
#define _CHANNEL_H_
#include "Timestamp.h"
#include <functional>
namespace wwj
{
class EventLoop;
class Channel
{  
public:
    typedef std::function<void()> EventCallback;
    typedef std::function<void(Timestamp)> ReadEventCallback;
    Channel(EventLoop* loop,int fd);
    ~Channel();
    void setReadCallback(const ReadEventCallback& cb){readCallback_=cb;}
    void setWriteCallback(const EventCallback& cb){writeCallback_=cb;}
    void setErrorCallback(const EventCallback& cb){errorCallback_=cb;}
    void setCloseCallback(const EventCallback& cb){closeCallback_=cb;}

    void handleEvent(Timestamp receiveTime);
    void set_revents(int revt){revents_=revt;}
    void enableReading(){events_|=kReadEvent;update();  }
    void enableWriting() { events_ |= kWriteEvent; update(); }
    void disableWriting() { events_ &= ~kWriteEvent; update(); }
    void disableAll() { events_ = kNoneEvent; update(); }
    bool isWriting() const { return events_ & kWriteEvent; }
    int fd() const {return fd_;}
    int events() const {return events_;}
    int index(){return index_;}
    void set_index(int index){index_=index;}
    bool isNoneEvent() const {return events_==kNoneEvent;}
    EventLoop* ownerLoop() { return loop_; }
private:
    void update();

    static const int kNoneEvent;
    static const int kReadEvent;
    static const int kWriteEvent;
    int index_;
    EventLoop* loop_;
    const int fd_;
    bool eventHandling_;
    int events_;
    int revents_;

    ReadEventCallback readCallback_;
    EventCallback writeCallback_;
    EventCallback errorCallback_;
    EventCallback closeCallback_;
};




}
#endif
#include "Channel.h"
#include "Logging.h"
#include <poll.h>
#include "EventLoop.h"
using namespace wwj;

const int Channel::kNoneEvent=0;
const int Channel::kReadEvent=POLLPRI|POLLIN;
const int Channel::kWriteEvent=POLLOUT;

Channel::Channel(EventLoop* loop,int fd)
:eventHandling_(false),
fd_(fd),
loop_(loop),
events_(0),
revents_(0),
index_(-1)
{

}
Channel::~Channel()
{

}

void Channel::update()
{
    loop_->updateChannel(this);
}

void Channel::handleEvent(Timestamp receiveTime)
{
    eventHandling_=true;
    //描述符不是打开的文件
    if(revents_&POLLNVAL)
    {
        LOG_TRACE<<"Channel::handle_event() POLLNVAL";
    }
    //挂起
    if((revents_&POLLHUP)&&!(revents_&POLLIN))
    {
        LOG_TRACE<<"Channel::handle_event() POLLHUP";
        if(closeCallback_)
        {
            closeCallback_();
        }
    }
    //发生错误
    if(revents_&(POLLERR|POLLNVAL))
    {
        LOG_TRACE<<"Channel::handle_event() POLLERR|POLLNVAL";
        if(errorCallback_)
        {
            errorCallback_();
        }
    }
    //优先数据，普通数据，对端关闭(读0)
    if(revents_&(POLLIN | POLLPRI | POLLRDHUP))
    {
        LOG_TRACE<<"Channel::handle_event() POLLIN | POLLPRI | POLLRDHUP";
        if(readCallback_)
        {
            readCallback_(receiveTime);
        }
    }
    if(revents_&POLLOUT)
    {
        LOG_TRACE<<"Channel::handle_event() POLLOUT";
        if(writeCallback_)
        {
            writeCallback_();
        }
    }
    eventHandling_=false;
}
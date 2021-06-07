#include "Poller.h"
#include <poll.h>
#include "Logging.h"
#include "Channel.h"
using namespace wwj;

Poller::Poller(EventLoop * loop)
:ownerLoop_(loop)
{
    LOG_TRACE<<"Poller::Poller() end";
}
Poller::~Poller()
{

}
void Poller::updateChannel(Channel* channel)
{
    LOG_TRACE<<"Poller::updateChannel  fd="<<channel->fd()<<"event="<<channel->events();
    if(channel->index()<0)
    {
        LOG_TRACE<<"1";
        struct pollfd pfd;
        pfd.fd=channel->fd();
        pfd.events=static_cast<short>(channel->events());
        pfd.revents=0;
        LOG_TRACE<<"2";
        pollfds_.push_back(pfd);
        LOG_TRACE<<"3";
        int idx = static_cast<int>(pollfds_.size())-1;
        LOG_TRACE<<"4";
        channel->set_index(idx);
        channels_[pfd.fd]=channel;
        LOG_TRACE<<"5";
    }else{
        int idx =channel->index();
        struct pollfd& pfd = pollfds_[idx];
        pfd.events=static_cast<short>(channel->events());
        pfd.revents=0;
        if(channel->isNoneEvent())
        {
            pfd.fd = -channel->fd()-1;
        }
    }
    LOG_TRACE<<"Poller::updateChannel  end";
}
Timestamp Poller::poll(int timeoutMs,ChannelList* activeChannels)
{
    int numEvents  = ::poll(&*pollfds_.begin(),pollfds_.size(),timeoutMs);
    Timestamp now(Timestamp::now());
    if(numEvents>0)
    {
        fillActiveChannels(numEvents,activeChannels);
        LOG_TRACE<<numEvents<<" events happended";
    }else if(numEvents == 0)
    {
        LOG_TRACE<<"nothing happended";
    }else
    {
        LOG_SYSERR<<"Poller::poll()";
    }
    return now;
}

void Poller::fillActiveChannels(int numEvents,ChannelList* activeChannels)const
{
    for(PollFdList::const_iterator pfd=pollfds_.begin();
    pfd!=pollfds_.end()&&numEvents>0;++pfd)
    {
        if(pfd->revents>0)
        {
            --numEvents;
            ChannelMap::const_iterator ch=channels_.find(pfd->fd);
            Channel* channel = ch->second;
            channel->set_revents(pfd->revents);
            activeChannels->push_back(channel);
        }
    }
}

void Poller::removeChannel(Channel* channel)
{
  LOG_TRACE << "fd = " << channel->fd();
  int idx = channel->index();
  const struct pollfd& pfd = pollfds_[idx]; (void)pfd;
  size_t n = channels_.erase(channel->fd());
  if (implicit_cast<size_t>(idx) == pollfds_.size()-1) 
  {
    pollfds_.pop_back();
  } else 
  {
    int channelAtEnd = pollfds_.back().fd;
    iter_swap(pollfds_.begin()+idx, pollfds_.end()-1);
    if (channelAtEnd < 0) 
    {
      channelAtEnd = -channelAtEnd-1;
    }
    channels_[channelAtEnd]->set_index(idx);
    pollfds_.pop_back();
  }
}
#ifndef _POLLER_H_
#define _POLLER_H_
#include "EventLoop.h"
#include <vector>
#include <map>
struct pollfd;
namespace wwj
{
class Channel;
class Poller
{
public:
    typedef std::vector<Channel*> ChannelList;
    Poller(EventLoop* loop);
    ~Poller();
    Timestamp poll(int timeoutMs,ChannelList* activeChannels);
    void updateChannel(Channel* channel);
    void removeChannel(Channel* channel);
private:
    void fillActiveChannels(int numEvents,ChannelList* activeChannels) const;
    typedef std::vector<struct pollfd> PollFdList;
    PollFdList pollfds_;
    typedef std::map<int,Channel*> ChannelMap;
    ChannelMap channels_;
    EventLoop* ownerLoop_;
};

}

#endif
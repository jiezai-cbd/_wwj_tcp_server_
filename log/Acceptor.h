#ifndef _ACCEPTOR_H_
#define _ACCEPTOR_H_
#include "Channel.h"
#include "Socket.h"
#include <functional>
namespace wwj
{
class EventLoop;
class InetAddress;

class Acceptor
{
public:
    typedef std::function<void (int sockfd,const InetAddress&)> NewConnectionCallback;
    Acceptor(EventLoop* loop,const InetAddress& listenAddr);
    void setNewConnectionCallback(const NewConnectionCallback& cb)
    { 
        newConnectionCallback_ = cb; 
    }

    bool listenning() const 
    { 
        return listenning_; 
    }
    void listen();
private:

    void handleRead();
    EventLoop* loop_;
    Socket acceptSocket_;
    Channel acceptChannel_;
    NewConnectionCallback newConnectionCallback_;
    bool listenning_;

};

}
#endif
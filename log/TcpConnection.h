#ifndef _TCPCONNECTION_H_
#define _TCPCONNECTION_H_
#include "Buffer.h"
#include "InetAddress.h"
#include <memory>
#include "Callbacks.h"
#include "Timestamp.h"
namespace wwj
{
class Channel;
class EventLoop;
class Socket;

class TcpConnection:public std::enable_shared_from_this<TcpConnection>
{
public:
    TcpConnection(EventLoop* loop,const std::string& name,int sockfd,
    const InetAddress& localAddr,const InetAddress& peerAddr);
    ~TcpConnection();
    void shutdown();
    void send(const std::string& message);

    EventLoop* getLoop() const { return loop_; }
    const std::string& name() const { return name_; }
    const InetAddress& localAddress() { return localAddr_; }
    const InetAddress& peerAddress() { return peerAddr_; }
    bool connected() const { return state_ == kConnected; }

    void setConnectionCallback(const ConnectionCallback& cb)
    { 
        connectionCallback_ = cb; 
    }

    void setMessageCallback(const MessageCallback& cb)
    { 
        messageCallback_ = cb; 
    }

    void setWriteCompleteCallback(const WriteCompleteCallback& cb)
    { 
        writeCompleteCallback_ = cb; 
    }

    void setCloseCallback(const CloseCallback& cb)
    { 
        closeCallback_ = cb; 
    }

 
    void connectEstablished();  

    void connectDestroyed();  


private:
    enum StateE 
    { 
        kConnecting, 
        kConnected, 
        kDisconnecting, 
        kDisconnected, 
    };
    void handleRead(Timestamp receiveTime);
    void handleWrite();
    void handleClose();
    void handleError();
    void shutdownInLoop();
    void sendInLoop(const std::string& message);
    void setState(StateE s) { state_ = s; }
    StateE state_;
    EventLoop* loop_;
    std::string name_;
    std::unique_ptr<Socket> socket_;
    std::unique_ptr<Channel> channel_;
    InetAddress localAddr_;
    InetAddress peerAddr_;
    Buffer inputBuffer_;
    Buffer outputBuffer_;
    ConnectionCallback connectionCallback_;
    MessageCallback messageCallback_;
    WriteCompleteCallback writeCompleteCallback_;
    CloseCallback closeCallback_;
    
};


    
}


#endif
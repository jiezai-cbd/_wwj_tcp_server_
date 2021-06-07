#include "TcpConnection.h"
#include "Logging.h"
#include "Channel.h"
#include "EventLoop.h"
#include "Socket.h"
#include "SocketsOps.h"
#include <errno.h>
#include <stdio.h>
#include <functional>
#include <memory>

using namespace wwj;
TcpConnection::TcpConnection(EventLoop* loop,const std::string& nameArg,int sockfd,
 const InetAddress& localAddr,const InetAddress& peerAddr)
 :loop_(loop),
 name_(nameArg),
 socket_(new Socket(sockfd)),
 channel_(new Channel(loop,sockfd)),
 localAddr_(localAddr),
 peerAddr_(peerAddr_)
 {
    LOG_TRACE<<"TcpConnection::TcpConnection()";
    channel_->setReadCallback(std::bind(&TcpConnection::handleRead,this,std::placeholders::_1));
    channel_->setWriteCallback(std::bind(&TcpConnection::handleWrite, this));
    channel_->setCloseCallback(std::bind(&TcpConnection::handleClose, this));
    channel_->setErrorCallback(std::bind(&TcpConnection::handleError, this));
 }

TcpConnection::~TcpConnection()
{
  LOG_DEBUG << "TcpConnection::~TcpConnection()";
}

void TcpConnection::handleRead(Timestamp receiveTime)
{
  int savedErrno = 0;
  ssize_t n = inputBuffer_.readFd(channel_->fd(), &savedErrno);
  if (n > 0) 
  {
    
  } else if (n == 0) 
  {
    handleClose();
  } else 
  {
    errno = savedErrno;
    LOG_SYSERR << "TcpConnection::handleRead";
    handleError();
  }
}

void TcpConnection::handleWrite()
{
  if (channel_->isWriting()) 
  {
    ssize_t n = ::write(channel_->fd(),outputBuffer_.peek(),outputBuffer_.readableBytes());
    if (n > 0) 
    {
      outputBuffer_.retrieve(n);
      if (outputBuffer_.readableBytes() == 0) 
      {
        channel_->disableWriting();
        
        if (state_ == kDisconnecting) 
        {
          shutdownInLoop();
        }
      } else 
      {
        LOG_TRACE << "I am going to write more data";
      }
    } else 
    {
      LOG_SYSERR << "TcpConnection::handleWrite";
    }
  } else 
  {
    LOG_TRACE << "Connection is down, no more writing";
  }
}

void TcpConnection::handleClose()
{
  LOG_TRACE << "TcpConnection::handleClose state = " << static_cast<int>(state_);
  channel_->disableAll();
}

void TcpConnection::handleError()
{
  int err = sockets::getSocketError(channel_->fd());
  LOG_ERROR << "TcpConnection::handleError ";
}

void TcpConnection::shutdown()
{
  if (state_ == kConnected)
  {
    setState(kDisconnecting);
    loop_->runInLoop(std::bind(&TcpConnection::shutdownInLoop, this));
  }
}

void TcpConnection::shutdownInLoop()
{
  if (!channel_->isWriting())
  {
    socket_->shutdownWrite();
  }
}


void TcpConnection::send(const std::string& message)
{
  if (state_ == kConnected) 
  {
    if (loop_->isInLoopThread()) 
    {
      sendInLoop(message);
    } else 
    {
      loop_->runInLoop(std::bind(&TcpConnection::sendInLoop, this, message));
    }
  }
}

void TcpConnection::sendInLoop(const std::string& message)
{
  ssize_t nwrote = 0;
  //没有关注可写时间，并且没有要写出去的数据，防止数据被打乱顺序
  if (!channel_->isWriting() && outputBuffer_.readableBytes() == 0) 
  {
    nwrote = ::write(channel_->fd(), message.data(), message.size());
    if (nwrote >= 0) 
    {
      if (implicit_cast<size_t>(nwrote) < message.size()) 
      {
        LOG_TRACE << "I am going to write more data";
      } 
      
    } else 
    {
      nwrote = 0;
      if (errno != EWOULDBLOCK) 
      {
        LOG_SYSERR << "TcpConnection::sendInLoop";
      }
    }
  }
  if (implicit_cast<size_t>(nwrote) < message.size()) 
  {
    outputBuffer_.append(message.data()+nwrote, message.size()-nwrote);
    if (!channel_->isWriting()) 
    {
      channel_->enableWriting();
    }
  }
}

void TcpConnection::connectEstablished()
{
  setState(kConnected);
  channel_->enableReading();
  connectionCallback_(shared_from_this());
}

void TcpConnection::connectDestroyed()
{
  setState(kDisconnected);
  channel_->disableAll();
  connectionCallback_(shared_from_this());
  loop_->removeChannel(channel_.get());
}
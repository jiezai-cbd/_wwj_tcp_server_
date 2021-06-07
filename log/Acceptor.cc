#include "Acceptor.h"

#include "Logging.h"
#include "EventLoop.h"
#include "InetAddress.h"
#include "SocketsOps.h"

#include <functional>

using namespace wwj;

Acceptor::Acceptor(EventLoop* loop, const InetAddress& listenAddr)
:loop_(loop),
acceptSocket_(sockets::createNonblockingOrDie()),
acceptChannel_(loop,acceptSocket_.fd()),
listenning_(false)
{
    acceptSocket_.setReuseAddr(true);
    acceptSocket_.bindAddress(listenAddr);
    acceptChannel_.setReadCallback(std::bind(&Acceptor::handleRead, this));
}


void Acceptor::listen()
{
  listenning_ = true;
  acceptSocket_.listen();
  acceptChannel_.enableReading();
}

void Acceptor::handleRead()
{
  InetAddress peerAddr(0);
  int connfd = acceptSocket_.accept(&peerAddr);
  if (connfd >= 0) 
  {
    if (newConnectionCallback_) 
    {
      newConnectionCallback_(connfd, peerAddr);
    } else 
    {
      sockets::close(connfd);
    }
  }
}

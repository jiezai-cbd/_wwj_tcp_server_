#ifndef _CALLBACKS_H_
#define _CALLBACKS_H_
#include <functional>
#include <memory>
#include "Timestamp.h"

namespace wwj
{

class Buffer;
class TcpConnection;
typedef std::shared_ptr<TcpConnection> TcpConnectionPtr;
typedef std::function<void()> TimerCallback;
typedef std::function<void (const TcpConnectionPtr&)> ConnectionCallback;
typedef std::function<void (const TcpConnectionPtr&,Buffer* buf,Timestamp)> MessageCallback;
typedef std::function<void (const TcpConnectionPtr&)> WriteCompleteCallback;
typedef std::function<void (const TcpConnectionPtr&)> CloseCallback;
}

#endif
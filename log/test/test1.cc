#include "Channel.h"
#include<stdio.h>
#include <unistd.h>
#include "EventLoop.h"
#include <sys/timerfd.h>
#include <string.h>
#include "Logging.h"
#include <functional>

wwj::EventLoop* g_loop;
int timerfd;
void timeout(wwj::Timestamp receiveTime)
{
    LOG_TRACE<<"Channel::ReadEventCallback()";
    uint64_t exp = 0;
    read(timerfd, &exp, sizeof(uint64_t)); 
    //g_loop->quit();
}

int cnt = 0;

void print(const char* msg)
{
  printf("msg %s %s\n", wwj::Timestamp::now().toString().c_str(), msg);
  if (++cnt == 20)
  {
    g_loop->quit();
  }
}

wwj::TimerId toCancel;

void cancelSelf()
{
  print("cancelSelf()");
  g_loop->cancel(toCancel);
}

int main()
{
    /*
    LOG_TRACE<<"这是LOG_TRACE";
    LOG_DEBUG<<"这是LOG_DEBUG";
    LOG_INFO<<"这是LOG_INFO";
    wwj::Logger::setLogLevel(wwj::Logger::LogLevel::TRACE);
    int level = static_cast<int>(wwj::Logger::logLevel());
    wwj::Logger::setLogLevel(wwj::Logger::LogLevel::DEBUG);
    level = static_cast<int>(wwj::Logger::logLevel());
    LOG_INFO<<"DEBUG level:"<<level;
    wwj::Logger::setLogLevel(wwj::Logger::LogLevel::INFO);
    level = static_cast<int>(wwj::Logger::logLevel());
    LOG_INFO<<"INFO level:"<<level;
    wwj::Logger::setLogLevel(wwj::Logger::LogLevel::WARN);
    level = static_cast<int>(wwj::Logger::logLevel());
    LOG_INFO<<"WARN level:"<<level;
    wwj::Logger::setLogLevel(wwj::Logger::LogLevel::ERROR);
    level = static_cast<int>(wwj::Logger::logLevel());
    LOG_INFO<<"ERROR level:"<<level;
    wwj::Logger::setLogLevel(wwj::Logger::LogLevel::FATAL);
    level = static_cast<int>(wwj::Logger::logLevel());
    LOG_INFO<<"FATAL level:"<<level;
    LOG_TRACE<<"这是LOG_TRACE";
    LOG_DEBUG<<"这是LOG_DEBUG";
    LOG_INFO<<"这是LOG_INFO";
    

   wwj::Logger::setLogLevel(wwj::Logger::LogLevel::INFO);
   LOG_TRACE<<"不应该出现的消息";
   wwj::Logger::setLogLevel(wwj::Logger::LogLevel::TRACE);
   LOG_TRACE<<"正常出现的消息";
   LOG_TRACE<<"线程name:"<<wwj::CurrentThread::name();
   LOG_TRACE<<"线程是否是主线程:"<<static_cast<int>(wwj::CurrentThread::isMainThread());
   LOG_TRACE<<"线程id:"<<static_cast<int>(wwj::CurrentThread::tid());
   */  
    
    wwj::EventLoop loop;
    g_loop=&loop;
     timerfd= ::timerfd_create(CLOCK_MONOTONIC, TFD_NONBLOCK | TFD_CLOEXEC);
    wwj::Channel channel(&loop,timerfd);
    channel.setReadCallback(timeout);
    channel.enableReading();
    

    struct itimerspec howlong;
    bzero(&howlong, sizeof howlong);
    //howlong.it_value.tv_sec = 5;
    howlong.it_value.tv_sec = 2;
    //howlong.it_value.tv_nsec = 2;
    howlong.it_interval.tv_sec = 2;
    
    ::timerfd_settime(timerfd, 0, &howlong, NULL);

    loop.loop();
    
    

/*
    wwj::EventLoop loop;
    g_loop = &loop;

    loop.loop();
    */
/*
    print("main");
    loop.runAfter(1, std::bind(print, "once1"));
    loop.runAfter(1.5, std::bind(print, "once1.5"));
    loop.runAfter(2.5, std::bind(print, "once2.5"));
    loop.runAfter(3.5, std::bind(print, "once3.5"));
    wwj::TimerId t = loop.runEvery(2, std::bind(print, "every2"));
    loop.runEvery(3, std::bind(print, "every3"));
    loop.runAfter(10, std::bind(&wwj::EventLoop::cancel, &loop, t));
    toCancel = loop.runEvery(5, cancelSelf);

    loop.loop();
    print("main loop exits");
    */

   
    



    return 0;
}
#ifndef _LOGGING_H_
#define _LOGGING_H_

#include "LogStream.h"
#include "Timestamp.h"
namespace wwj
{

class Logger
{
public:
    enum LogLevel
    {
        TRACE=0,
        DEBUG,
        INFO,
        WARN,
        ERROR,
        FATAL,
        NUM_LOG_LEVELS,

    };
    
    Logger(const char* file,int line,LogLevel leve,const char* func);
    Logger(const char* file ,int line);
    Logger(const char* file ,int line,LogLevel level);
    Logger(const char* file, int line, bool toAbort);
    ~Logger();
    
    static LogLevel logLevel();
    LogStream& stream(){return impl_.stream_;}

    static void setLogLevel(LogLevel level);
    typedef void (*OutputFunc)(const char* msg,int len);
    typedef void (*FlushFunc)();
    static void setOutput(OutputFunc);
    static void setFlush(FlushFunc);

    
private:
class Impl
{
public:
    typedef Logger::LogLevel LogLevel;
    Impl(LogLevel level,int old_errno,const char* file,int line);
    LogStream stream_;
    LogLevel level_;
    void formatTime();

    void finish();
    Timestamp time_;
    int line_;
    const char* basename_;
    const char* fullname_;
};
    Impl impl_;
    
};

#define LOG_TRACE if(wwj::Logger::logLevel()<=wwj::Logger::TRACE)\
wwj::Logger(__FILE__,__LINE__,wwj::Logger::TRACE,__func__).stream()
#define LOG_DEBUG if(wwj::Logger::logLevel()<= wwj::Logger::DEBUG)\
wwj::Logger(__FILE__,__LINE__,wwj::Logger::DEBUG,__func__).stream()
#define LOG_INFO if(wwj::Logger::logLevel()<= wwj::Logger::INFO)\
wwj::Logger(__FILE__,__LINE__).stream()
#define LOG_WARN wwj::Logger(__FILE__,__LINE__,wwj::Logger::WARN).stream()
#define LOG_ERROR wwj::Logger(__FILE__,__LINE__,wwj::Logger::ERROR).stream()
#define LOG_FATAL wwj::Logger(__FILE__,__LINE__,wwj::Logger::FATAL).stream()
#define LOG_SYSERR wwj::Logger(__FILE__,__LINE__,false).stream()
#define LOG_SYSFATAL wwj::Logger(__FILE__,__LINE__,true).stream()

const char* strerror_tl(int savedErrno);
template<typename To, typename From>
To implicit_cast(From const &f) {
    return f;
}

}

#endif
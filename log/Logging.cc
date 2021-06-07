#include "Logging.h"
#include <stdio.h>
#include <sstream>
#include <errno.h>

namespace wwj
{
char t_time[32];
char t_errnobuf[512];
const char* strerror_tl(int savedErrno)
{
    return strerror_r(savedErrno,t_errnobuf,sizeof t_errnobuf);
}
void defaultOutput(const char * msg,int len)
{
    size_t n = fwrite(msg,1,len,stdout);
    (void)n;
}
void defaultFlush()
{
    fflush(stdout);
}
Logger::OutputFunc g_output=defaultOutput;
Logger::FlushFunc g_flush=defaultFlush;

const char* LogLevelName[Logger::NUM_LOG_LEVELS]=
{
    "TRACE ",
    "DEBUG ",
    "INFO  ",
    "WARN  ",
    "ERROR ",
    "FATAL ",
};
Logger::LogLevel initLogLevel()
{
    return Logger::TRACE;   
}
Logger::LogLevel g_logLevel = initLogLevel();
}

using namespace wwj;

Logger::Impl::Impl(LogLevel level ,int savedErrno,const char* file,int line)
:stream_(),
level_(level),
line_(line),
fullname_(file),
basename_(NULL),
time_(Timestamp::now())
{
    
    //printf("impl constructor : line:%d,level:%d\n",line,level);
    const char* path_sep_pos = strrchr(fullname_,'/');
    basename_=(path_sep_pos!=NULL)?path_sep_pos+1:fullname_;
    formatTime();
    stream_<<T(LogLevelName[level],6);
    if(savedErrno!=0)
    {
        stream_<<strerror_tl(savedErrno)<<" (errno="<<savedErrno<<") ";
    }
}

void Logger::Impl::formatTime()
{
    int64_t microSecondsSinceEpoch = time_.microSecondsSinceEpoch();
    time_t seconds = static_cast<time_t>(microSecondsSinceEpoch/1000000);
    int microseconds = static_cast<int>(microSecondsSinceEpoch%1000000);
    struct tm tm_time;
    ::gmtime_r(&seconds,&tm_time);
    int len = snprintf(t_time,sizeof(t_time),"%4d%02d%02d %02d:%02d:%02d",
    tm_time.tm_year+1900,tm_time.tm_mon+1,tm_time.tm_mday,
    tm_time.tm_hour,tm_time.tm_min,tm_time.tm_sec);
    Fmt us(".%06dZ ",microseconds);
    stream_<<T(t_time,17)<<T(us.data(),9);
    
}
void Logger::Impl::finish()
{
    //printf("finish function : %d\n",line_);
    stream_<<" - "<<basename_<<':'<<line_<<'\n';
}

Logger::LogLevel Logger::logLevel()
{
    //printf("loglevel:%d/n",);
    return g_logLevel;
}


void Logger::setLogLevel(LogLevel level)
{
  g_logLevel = level;
}

Logger::Logger(const char * file,int line, LogLevel level,const char * func)
:impl_(level,0,file,line)
{
    //printf("logger constructor : used level:%d,the level have set : %d\n",level,logLevel());
    impl_.stream_<<func<<' ';
}
Logger::Logger(const char* file ,int line)
:impl_(INFO,0,file,line)
{

}

Logger::Logger(const char* file ,int line,LogLevel level)
:impl_(level,0,file,line)
{

}
Logger::Logger(const char* file,int line,bool toAbort)
:impl_(toAbort?FATAL:ERROR,errno,file,line)
{

}
Logger::~Logger()
{
    impl_.finish();
    const LogStream::Buffer& buf(stream().buffer());
    g_output(buf.data(),buf.length());
    if(impl_.level_==FATAL)
    {
        g_flush();
        abort();
    }
}

void Logger::setOutput(OutputFunc out)
{
    g_output=out;
}

void Logger::setFlush(FlushFunc flush)
{
    g_flush=flush;
}

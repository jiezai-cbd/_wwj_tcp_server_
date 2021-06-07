#include "Thread.h"
;
#include <unistd.h>
#include <sys/prctl.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <linux/unistd.h>
#include <memory>


namespace wwj
{
namespace CurrentThread
{
    __thread const char* t_threadName = "main";
}
}
namespace
{
/*
pid_t gettid()
{
  return static_cast<pid_t>(::syscall(SYS_gettid));
}
*/

struct ThreadData
{
  typedef wwj::Thread::ThreadFunc ThreadFunc;
  ThreadFunc func_;
  std::string name_;
  pid_t* wkTid_;

  ThreadData(const ThreadFunc& func,const std::string& name,pid_t* tid)
    : func_(func),name_(name),wkTid_(tid)
  { }

  void runInThread()
  {
    *wkTid_ = wwj::CurrentThread::tid();
    wwj::CurrentThread::t_threadName = name_.empty() ? "wwjThread" : name_.c_str();
    ::prctl(PR_SET_NAME, wwj::CurrentThread::t_threadName);
    func_(); 
    wwj::CurrentThread::t_threadName = "finished";
  }
};

void* startThread(void* obj)
{
  ThreadData* data = static_cast<ThreadData*>(obj);
  data->runInThread();
  delete data;
  return NULL;
}


}

using namespace wwj;
pid_t CurrentThread::tid()
{
    return gettid();
}

bool CurrentThread::isMainThread()
{
    return tid()==::getpid();
}

const char* CurrentThread::name()
{
    return t_threadName;
}

std::atomic<int32_t> Thread::numCreated_;

Thread::Thread(const ThreadFunc& func, const std::string& n)
  : started_(false),
    joined_(false),
    pthreadId_(0),
    tid_(0),
    func_(func),
    name_(n)
{
  numCreated_++;
}

Thread::~Thread()
{
  if (started_ && !joined_)
  {
    pthread_detach(pthreadId_);
  }
}

void Thread::join()
{
  joined_ = true;
  pthread_join(pthreadId_, NULL);
}

void Thread::start()
{
  started_ = true;
  ThreadData* data = new ThreadData(func_, name_, &tid_);
  if (pthread_create(&pthreadId_, NULL, &startThread, data))
  {
    started_ = false;
    delete data;
    abort();
  }
}
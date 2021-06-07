#include "LogStream.h"
#include <algorithm>
#include <stdio.h>
namespace wwj
{
namespace detail
{
const char digits[]="9876543210123456789";
const char * zero = digits+9;
template<typename T>
size_t convert(char buf[],T value)
{
    T i = value;
    char *p = buf;
    do
    {
        int lsd = i%10;
        i=i/10;
        *p++=zero[lsd];
    } while (i!=0);

    if(value<0)
    {
        *p++='-';
    }
    *p='\0';
    std::reverse(buf,p);
    return p-buf;
    
}
}

}

using namespace wwj;
using namespace wwj::detail;

template<typename T>
void LogStream::formatInteger(T v)
{
    if(buffer_.avail()>=kMaxNumericSize)
    {
        size_t len = convert(buffer_.current(),v);
        buffer_.add(len);
    }
}

LogStream& LogStream::operator<<(int v)
{
  formatInteger(v);
  return *this;
}

template<typename T>
Fmt::Fmt(const char* fmt,T val)
{
    length_=snprintf(buf_,sizeof buf_,fmt,val);

}

template Fmt::Fmt(const char* fmt, int);
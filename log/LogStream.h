#ifndef _LOGSTREAM_H_
#define _LOGSTREAM_H_
#include <string.h>
#include <assert.h>
namespace wwj
{

namespace detail
{
const int kSmallBuffer = 4000;

template<int SIZE>
class FixedBuffer 
{
public:
    FixedBuffer():cur_(data_)
    {

    }
    ~FixedBuffer()
    {

    }

    void append(const char* buf,int len)
    {
        if(avail()>len)
        {
            memcpy(cur_,buf,len);
            cur_+=len;
        }
    }
    const char* data() const{return data_;}
    int length() const {return cur_-data_;}

    int avail() const {return static_cast<int>(end()-cur_);}
    char * current(){return cur_;}
    void add(size_t len){cur_+=len;}
private:
    const char * end() const {return data_ + sizeof data_;} 

    char data_[SIZE];
    char* cur_;
}; 

}
class T
{
public:
    T(const char * str,int len)
    :str_(str),len_(len)
    {
        assert(strlen(str) == len_);
    }
    const char* str_;
    const size_t len_;
};
class LogStream
{
    typedef LogStream self;
    
public:
    typedef wwj::detail::FixedBuffer<detail::kSmallBuffer> Buffer;
    self& operator<<(char v)
    {
        buffer_.append(&v,1);
        return *this;
    }
    self& operator<<(const char * v)
    {
        buffer_.append(v,strlen(v));
        return *this;
    }
    self& operator<<(const T& v)
    {
        buffer_.append(v.str_,v.len_);
        return *this;
    }

    self& operator<<(int);
    const Buffer& buffer() const{return buffer_;}

private:

    template<typename T>
    void formatInteger(T);

    Buffer buffer_;

    static const int kMaxNumericSize=32;
};
class Fmt
{
public:
    template<typename T>
    Fmt(const char * fmt,T val);
    const char* data() const {return buf_;}
    int length() const {return length_;}

private:
     char buf_[32];
     int length_;
};

}

#endif
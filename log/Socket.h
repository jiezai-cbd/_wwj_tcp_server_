#ifndef _SOCKET_H_
#define _SOCKET_H_

namespace wwj
{
class InetAddress;
class Socket
{
public:
    Socket(int sockfd):sockfd_(sockfd)
    {

    }
    ~Socket();

    int fd() const {return sockfd_;}

    void bindAddress(const InetAddress& localaddr);
    void listen();
    int accept(InetAddress* peeraddr);
    void setReuseAddr(bool on);
    void shutdownWrite();
    void setTcpNoDelay(bool on);


private:
    const int sockfd_;
};
    
}


#endif
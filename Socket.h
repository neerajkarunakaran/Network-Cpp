#ifndef SOCKET_H
#define SOCKET_H

#include <iostream>
#include <cstdlib>
#include <utility>
#include <sys/types.h>
#include <netdb.h>
#include <netinet/in.h>

namespace Tcp
{
    class Socket
    {
    public:

        Socket(int socket_);
        
        ~Socket();

        // base send and recv functions
        int sendData(const char *data_ , size_t size_);
        std::pair<const char *, int>  recvData();

    protected:
        
        int                   _socket;
        char                  _recvBuffer[4096];
    };
}

#endif

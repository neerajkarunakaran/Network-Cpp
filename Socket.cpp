
#include <cstring>
#include <cstring>
#include <cassert>
#include <error.h>
#include <unistd.h>

#include <arpa/inet.h>

#include "Socket.h"

namespace Tcp
{
    Socket::Socket(int socket_)
        :_socket(socket_)
    {
       
    }
    
    Socket::~Socket()
    {
        std::cout << "Closing socket" << std::endl;
        close(_socket);
    }
    
    int Socket::sendData(const char *data_,  size_t size_)
    {
        return send(_socket, data_, size_, 0);
    }

    std::pair<const char *, int> Socket::recvData()
    {
        std::memset(_recvBuffer, 0, 4096);
        std::pair<const char*, int> data;
        data.second = recv(_socket, _recvBuffer, 4096, 0);
        if(-1 == data.second)
            std::cout << strerror(errno) << std::endl;
        assert(-1 != data.second);
        
        data.first = _recvBuffer;
        return data;
    }
}
        

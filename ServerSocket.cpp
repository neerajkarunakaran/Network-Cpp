
#include <cstring>
#include <cassert>
#include <iostream>
#include <error.h>


#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>

#include <netdb.h>
#include <unistd.h>

#include "ServerSocket.h"

namespace Tcp
{
    
    ServerSocket::ServerSocket()
        :Socket(socket(AF_INET, SOCK_STREAM, 0))
    {
         _socket = socket(AF_INET, SOCK_STREAM, 0);
        if(-1 == _socket)
            std::cerr << strerror(errno) << std::endl;
        assert(-1 != _socket);
    }
       
    bool ServerSocket::bindPort(uint16_t port_)
    {
        struct sockaddr_in address;
        memset(&address, 0, sizeof address);
        address.sin_family = AF_INET;
        address.sin_port = htons(port_);
        address.sin_addr.s_addr = INADDR_ANY;
        
        int ret;

        //set socket opt
         setsockopt(_socket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &ret, sizeof ret);

        //bind
        ret = bind(_socket, (struct sockaddr*)&address, sizeof address);
        
        return (-1 != ret) ? true : false;
    }
    
    bool ServerSocket::listenClients(int backLogs_)
    {
        int ret;

        //listen
        ret = listen(_socket, backLogs_);
        
        return ret < 0 ? false: true;
    }
    
    std::shared_ptr<Socket> ServerSocket::acceptClient()
    {
        int newSocket;
        
        struct sockaddr_storage clientAddress;
        socklen_t size;

        //accept client here
        newSocket = accept(_socket, (struct sockaddr*)&clientAddress, &size);
        if(-1 == newSocket)
            std::cerr << strerror(errno) << std::endl;
        assert(-1 != newSocket);
        

        //print client address
        char addr[16];
        uint16_t port;
        
        if(AF_INET == clientAddress.ss_family)
        {
             struct sockaddr_in *p = (struct sockaddr_in*)&clientAddress;
             inet_ntop(p->sin_family, &(p->sin_addr.s_addr), addr, INET_ADDRSTRLEN);
             port = ntohs(p->sin_port);
        }
        else
        {
            struct sockaddr_in6 *p = (struct sockaddr_in6*)&clientAddress;
            inet_ntop(p->sin6_family, &(p->sin6_addr.s6_addr), addr, 16);
            port = ntohs(p->sin6_port);
        }
        std::cout << "accepted client " << addr << " : " << port << std::endl;

        std::shared_ptr<Socket> connSocket = std::make_shared<Socket>(newSocket);
        
        return connSocket;
    }
                                                                   
}


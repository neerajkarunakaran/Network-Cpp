

#ifndef SERVERSOCKET_H
#define SERVERSOCKET_H

#include <memory>
#include "Socket.h"


namespace Tcp
{

    class ServerSocket: public Socket
    {
    public:

        ServerSocket();
        //bind
        bool bindPort(uint16_t port_);
        //listen
        bool listenClients(int backLogs_);
        //acccept
        std::shared_ptr<Socket> acceptClient();
       
    };
        
}
#endif

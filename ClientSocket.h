#ifndef CLIENTSOCKET_H
#define CLIENTSOCKET_H

#include <iostream>

#include "Serializer.h"
#include "Socket.h"


namespace Tcp
{
    class ClientSocket: public Socket
    {
    
    public:
        // constructor
        ClientSocket(const std::string& ip_, uint16_t port_, const std::string& clientName_);

        // processing functions
        bool makeConnection();
        bool sendMessage(const char* message_, std::size_t size);
        bool sendFile(std::string &fileName_, std::string & content_);
        void disconnect();

    private:
        bool sendHandshakeMessage();
        bool sendEverythingInSerializer();
    
        std::string            _ip;
        int                    _port;
        std::string            _clientName;
        Serializer             _serializer;
   
    };
}

#endif

  
        


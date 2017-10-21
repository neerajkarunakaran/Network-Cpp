#ifndef CONNECTIONHANDLER_H
#define CONNECTIONHANDLER_H

#include <cstddef>
#include <memory>

#include "Deserializer.h"
#include "FilesWriter.h"
#include "Socket.h"


namespace Tcp
{
     //Funct
    class ConnectionHandler
    {
    public:
        
        void handleConnection(std::shared_ptr<Socket> connectionSocket_);

    private:
        bool recvFile(const std::shared_ptr <Socket>& connectionSocket_);

        FilesWriter                     _filesWriter;
        Deserializer                    _deserializer;
        std::string                     _clientName;
    };
}

#endif

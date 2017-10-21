
#include <cassert>
#include <cstdlib>
#include <cstring>
#include <error.h>
#include <iostream>

#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <unistd.h>

#include "ClientSocket.h"

namespace Tcp
{
    ClientSocket::ClientSocket(const std::string& ip_, uint16_t port_, const std::string& clientName_)
        : Socket(socket(AF_INET, SOCK_STREAM, 0))
        , _ip(ip_)
        , _port(port_)
        , _clientName(clientName_)
    {
         _socket = socket(AF_INET, SOCK_STREAM, 0);
        if(-1 == _socket)
            std::cerr << strerror(errno) << std::endl;
        assert(-1 != _socket);
    }

    
    bool ClientSocket::makeConnection()
    {
        // checking if socket is valid
        if(-1 == _socket)
        {
            std::cerr << "Invalid socket. Please make sure limit of file descriptors not reached." << std::endl;
            return false;
        }
        
        // memsetting address to 0
        struct sockaddr_in address;
        memset(&address, 0, sizeof address);
        
        // connection and checking if it connects
        inet_pton(AF_INET, _ip.c_str(), &(address.sin_addr));
        address.sin_family = AF_INET;
        address.sin_port = htons(_port);

        int ret = connect(_socket, (sockaddr*)&address, sizeof address);

        if(-1 == ret)
        {
            std::cerr << "Failed creating connection. Check if server is up" << std::endl;
            return false;
        }
        
        if(!sendHandshakeMessage())
        {
            std::cerr << "Handshake failed!!!" << std::endl;
            return false;
        }

        return true;
    }

    bool ClientSocket::sendMessage(const char* message_, std::size_t size_)
    {
        int sizeSent = -1;
        if(-1 == (sizeSent = sendData(message_, size_)))
        {
            std::cerr << "Sending failed... retrying" << std::endl;
            sizeSent = sendData(message_, size_);
        }
        
        return (-1 == sizeSent)? false : true;
    }
    
    bool ClientSocket::sendFile(std::string &fileName_, std::string & content_)
    {
        std::cout << "Going to send file " << " : " << fileName_ << std::endl;
        std::cout << "File size : " << content_.size() << std::endl;

        // serializing data as per protocol
        _serializer.enqueueCharToStream('F');
        _serializer.enqueueIntToStream(fileName_.size());
        _serializer.enqueueStringToStream(fileName_);
        _serializer.enqueueIntToStream(content_.size());
        _serializer.enqueueStringToStream(content_);
        
        // sending data
        return sendEverythingInSerializer();                            
    }
        
    void ClientSocket::disconnect()
    {
        close(_socket);
    }
    
    bool ClientSocket::sendHandshakeMessage()
    {
        // creating handshake message
        _serializer.enqueueCharToStream('H');
        _serializer.enqueueIntToStream(_clientName.size());
        _serializer.enqueueStringToStream(_clientName);
        
        // sending data
        return sendEverythingInSerializer();   
    }
    
    bool ClientSocket::sendEverythingInSerializer()
    {
        while (_serializer.getStreamSize() > 0)
        {
            std::pair<const char *, std::size_t> bytesToBeSend = _serializer.dequeueFromStream(4096);

            if(-1 == sendData(bytesToBeSend.first, bytesToBeSend.second))
            {
            
                std::cerr << "Sending failed... Retrying" << std::endl;

                if(-1 == sendData(bytesToBeSend.first, bytesToBeSend.second))
                    return false;
            }

        }
        
        return true;
    }
}
    

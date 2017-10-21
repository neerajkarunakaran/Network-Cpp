



#include <iostream>
#include <memory>
#include <thread>
#include <utility>
#include <vector>

#include "ConnectionHandler.h"
#include "Socket.h"
#include "ServerSocket.h"


int main()
{

    // ask server port
    uint16_t serverPort;
    std::cout << "Enter port address : " ;
    std::cin >> serverPort;

    // binding
    Tcp::ServerSocket serverSocket;
    if(!serverSocket.bindPort(serverPort))
    {
        std::cerr << "Bind error... Check port available" << std::endl;
        return -1;
    }
    std::cout << "Binded Sucessfully" <<std::endl;
    
    // few variables
    typedef std::shared_ptr<std::thread>  ThreadPtr;
    std::vector<std::pair<ThreadPtr, Tcp::ConnectionHandler> > threadHandlerPairs;

    // listen clients
    if(serverSocket.listenClients(10))
    {
        std::cout << "Started........... Listening for connections" << std::endl;
        
        // accept client  
        while(std::shared_ptr<Tcp::Socket> connectionSocket = serverSocket.acceptClient())
        {
            threadHandlerPairs.push_back(std::make_pair(ThreadPtr(), Tcp::ConnectionHandler()));
            threadHandlerPairs.back().first = std::make_shared<std::thread>(std::thread(std::bind(&Tcp::ConnectionHandler::handleConnection,
                                                                    &threadHandlerPairs.back().second,
                                                                    connectionSocket)));
        }
    }
    else
    {
        std::cerr << "Listen error!!! Shutting down..."<< std::endl;
        return -1;
    }

    for(std::pair<ThreadPtr, Tcp::ConnectionHandler> & t : threadHandlerPairs)
    {
        t.first->join();
        
    }
    

    return 0;
}






































































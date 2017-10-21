
#include <iostream>


#include "Deserializer.h"
#include "FilesWriter.h"
#include "ConnectionHandler.h"

namespace Tcp
{

    bool ConnectionHandler::recvFile(const std::shared_ptr <Socket> & connectionSocket_)
    {

        std::string fileName;
        std::string content;
        int step=1;
        std::size_t expectedSize=1;

        while(step < 6)
        {
            while(_deserializer.getStreamSize() < expectedSize)
            {
                std::pair<const char *, int> clientDetails = connectionSocket_->recvData();

                //connection close
                if(0 >= clientDetails.second)
                {
                    std::cerr << "Client disconnected.... " << std::endl;
                    return false;
                }
                _deserializer.enqueuetoStream(clientDetails.first, clientDetails.second);      
            }

            std::pair<bool, char> c;
            std::pair<bool, int> size;
            std::pair<bool, std::string> string;
           
            switch(step)
            {
            case 1:
               
                // receiving and verifying first char
                c = _deserializer.dequeueCharFromStream();
                if('F' != c.second)
                {
                    std::cerr << "File mismatch.... Received char : " << c.second << std::endl;
                    return -1;
                }
                
                step = 2;
                expectedSize = sizeof (int);
                break;
             
            case 2:
                // receiving fileName size
                size  = _deserializer.dequeueIntFromStream();
                expectedSize = size.second;
                step = 3;
                break;
            case 3:
                //receiving fileName  here
                string = _deserializer.dequeueStringFromStream(expectedSize);
                fileName = string.second;
                
                expectedSize = sizeof (int);
                step = 4;
                break;
            case 4:           
                // receiving content  size
                size  = _deserializer.dequeueIntFromStream();
                expectedSize = size.second;
                
                step = 5;
                break;
            case 5:
                //receiving content here
                string = _deserializer.dequeueStringFromStream(expectedSize);
                content = string.second;
               
                if(!_filesWriter.createAndWriteToFile(fileName, content))
                    std::cerr << " write err " <<std::endl;
               
                step = 6;
                break;
            }
        }

        return true;
    }           
 
 
    void
    ConnectionHandler::handleConnection(std::shared_ptr <Socket> connectionSocket_)
    {
        // receiving handshake
        int step=1;
        std::size_t expectedSize=1;
        while(step < 4)
        {
            while(_deserializer.getStreamSize() < expectedSize)
            {
                std::pair<const char *, int> clientDetails = connectionSocket_->recvData();                                

                //connection close
                if(0 >= clientDetails.second)
                {
                    std::cerr << "Client disconnected.... " << std::endl;
                    return;
                }
                _deserializer.enqueuetoStream(clientDetails.first, clientDetails.second);      
            }
            std::pair<bool, char> c;
            std::pair<bool, int> size;
            std::pair<bool, std::string> string;
            switch(step)
            {
            case 1:
                
                // receiving and verifying first char
                c = _deserializer.dequeueCharFromStream();
                if('H' != c.second)
                {
                    std::cerr << "Handshake mismatch.. Received char : " << c.second << std::endl;
                    return;
                }

                step = 2;
                expectedSize = sizeof (int);
                break;
             
             case 2:
                
                 // receiving clientName size
                 size  = _deserializer.dequeueIntFromStream();
                 expectedSize = size.second;
                 step = 3;
               
                break;
                    
             case 3:
                
                // receiving client name
                string   = _deserializer.dequeueStringFromStream(expectedSize);
                _clientName = string.second;
                std::cout << "Handshake completed. Name of client connected : " << _clientName << std::endl;

                //client directory created here
                if(!_filesWriter.createDirectoryIfNotExistsAndMove(_clientName))
                    return;

                step = 4;
                
                break;
                    
             default:
                return;
            }
        }
    
        
       // recv files
        bool clientAlive = true;
        do
        {
            if(!recvFile(connectionSocket_))
                clientAlive = false;
        } while(clientAlive);
    }
}






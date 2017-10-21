
#include <iostream>
#include <string>
#include "Socket.h"
#include "ClientSocket.h"
#include "FilesReader.h"
#include "Serializer.h"


int main()
{
    
    
    // starting client and moving to client folder
    std::string clientName;
    std::cout << "Enter ClientName : ";
    std::cin >> clientName;
    
    Tcp::FilesReader filesReader;
    if(!filesReader.moveToDirectoryIfExists(clientName))
    {
        std::cerr << "Error!!! No directory found with client name. ClientName:" << clientName << std::endl;
        return -1;
    }
    
    // connecting to server
    std::string serverIP;
    uint16_t serverPort;
    std::cout << "Enter Server IP : ";
    std::cin >> serverIP;
    std::cout << "Enter Server Port : ";
    std::cin >> serverPort;  
    
    Tcp::ClientSocket clientSocket(serverIP, serverPort, clientName);
    if(!clientSocket.makeConnection())
    {
        std::cerr << "Failed connecting to server. Check address. ServerAddress:" << serverIP << ":" 
            << serverPort << std::endl;
        return -1;
    }
  
  
    // files sending loop
    bool haveFilesToSend = true;
    do
    {
        // checking if files need to be send
        char inp;
        std::cout << "Do you want to send files (y/n) : ";
        std::cin >> inp;
        
        // if no file is to be send shut down application
        if('n' == inp)
        {
            std::cout << "Disconnecting from server...." << std::endl;
            clientSocket.disconnect();
            
            std::cout << "Shutting down app..." << std::endl;
            haveFilesToSend = true;
            
            break;
        }
        // if file is to be send then read and send it
        else if('y' == inp)
        {
            // get filename
            std::string fileName;
            std::cout << "Enter filename : ";
            std::cin >> fileName;
            
            // try reading file
            std::pair<bool, std::string> fileToBeRead = filesReader.readFile(fileName);
           
            if(!fileToBeRead.first)
            {
                std::cerr << "The specified file does not exist. Filename : " << fileName << std::endl;
                continue;
            }
            
              if(!clientSocket.sendFile(fileName, fileToBeRead.second))
            {
                
                std::cerr << "Failed sending file to server. Filename : " << fileName << std::endl;
                continue;
                } 
        }
        // for wrong entry continue loop
        else
        {
            std::cerr << "Incorrect entry. Please try again." << std::endl;
            continue;
        }
    }while(haveFilesToSend);
    
    return 0;
}

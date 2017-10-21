    
#include <iostream>
#include <fstream>
#include<boost/filesystem.hpp>
#include "FilesWriter.h"

namespace Tcp
{

    FilesWriter::FilesWriter()
    {
        boost::filesystem::path curPath = boost::filesystem::current_path();
        _curDirectory = curPath.string();
    }

    bool FilesWriter::createDirectoryIfNotExistsAndMove(const std::string& directory_)
    {
        _curDirectory = _curDirectory + "/" + directory_;
        if(!boost::filesystem::exists(_curDirectory))
        {
            if(!boost::filesystem::create_directory(_curDirectory))
            {
                std::cerr << "Failed creating directory: "<< directory_ << std::endl;
                return false;
            }
            
        }
       
        return true;
    }

    bool FilesWriter::createAndWriteToFile(const std::string& fileName_, const std::string& buffer_)
    {
        
        std::string fullFileName = _curDirectory + "/" + fileName_;
        std::ofstream file;
        file.open(fullFileName, std::ios::binary);
        if(!file.is_open())
            return false;
        file << buffer_;
        file.close();
       
        return true;
    }
}

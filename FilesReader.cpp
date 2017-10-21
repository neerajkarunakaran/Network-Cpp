

#include <fstream>
#include <iostream>
#include <string>

#include <boost/filesystem.hpp>

#include "FilesReader.h"

namespace Tcp
{

    FilesReader::FilesReader()
    {
        boost::filesystem::path curPath = boost::filesystem::current_path();
        _curDirectory = curPath.string();
    }

    bool FilesReader::moveToDirectoryIfExists(const std::string& directory_)
    {
        
        std::string newDirectory = _curDirectory + "/" + directory_;
        
        if(!boost::filesystem::exists(newDirectory))
            return false;
        
        _curDirectory = newDirectory;
        
        return true;
    }

    std::pair<bool, std::string> FilesReader::readFile(const std::string& fileName_)
    {
        std::string fullFileName = _curDirectory + "/" + fileName_;
       
        if(!boost::filesystem::exists(fullFileName))
        {
            return std::make_pair(false, "");
        }
        
        std::pair<bool, std::string> retVal = std::make_pair(true, std::string(""));
        std::ifstream infile;
        infile.open(fullFileName, std::ios::binary);
        if(!infile.is_open())
        {
            retVal.first = false;
            retVal.second = " ";
            return retVal;
        }
        
        std::string content( (std::istreambuf_iterator<char>(infile) ),
                       (std::istreambuf_iterator<char>()    ) );
        retVal.first = true;
        retVal.second = content;
        
        infile.close();
        
        return retVal;
    }

           
}

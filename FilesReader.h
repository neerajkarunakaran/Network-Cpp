
#ifndef FILESREADER_H
#define FILESREADER_H

#include <string>


namespace Tcp
{
     class FilesReader
    {
    public:
        // constructor
        FilesReader(); // cur directory is identified here
        
        // directory search, create and move functions
        bool moveToDirectoryIfExists(const std::string& directory_);
        
        // file reader
        std::pair<bool, std::string> readFile(const std::string& filename_);
        
    private:
        std::string                         _curDirectory;
        
    };
}

#endif

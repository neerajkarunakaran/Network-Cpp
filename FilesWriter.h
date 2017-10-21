#ifndef FILESWRITER_H
#define FILESWRITER_H

#include <string>

#include <boost/optional.hpp>

namespace Tcp
{
     class FilesWriter
    {
    public:
        // constructor
        FilesWriter(); // cur directory is identified here
        
        // directory search, create and move functions
        bool createDirectoryIfNotExistsAndMove(const std::string& directory_);
        
        // file writer
        bool createAndWriteToFile(const std::string& fileName_, const std::string& buffer_);
        
    private:
        std::string                         _curDirectory;
        
    };
}

#endif

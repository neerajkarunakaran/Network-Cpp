
#ifndef DESERIALIZER_H
#define DESERIALIZER_H

#include <cstdlib>
#include <string>
#include <utility>


namespace Tcp
{
     class Deserializer
    {
    public:
        // constructor
         Deserializer(std::size_t initialBufferSize_ = 100000);
        
        // resetBuffer and Size
        void resetBuffer();
        std::size_t getStreamSize() const;
    
        // dequeueFromStream functions
        std::pair<bool, int> dequeueIntFromStream();
        std::pair<bool, std::string> dequeueStringFromStream(std::size_t size_);
        std::pair<bool, char> dequeueCharFromStream();
        
        // enqueue function
        void enqueuetoStream(const char* data_, std::size_t dataSize_);
        
    private:
        std::string            _stream;
        std::size_t            _startPos;
        
    };
}

#endif

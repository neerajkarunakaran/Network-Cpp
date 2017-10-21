#ifndef SERIALIZER_H
#define SERIALIZER_H

#include <iostream>

namespace Tcp
{
     class Serializer
    {
    public:
        // constructor
        Serializer(std::size_t initialBufferSize_ = 100000);
        
        // reset and size functions
        void resetBuffer();
        std::size_t getStreamSize() const;

        // enqueue functions
        void enqueueIntToStream(int value_);
        void enqueueStringToStream(const std::string & string_);
        void enqueueCharToStream(char char_);

        // dequque function
        std::pair<const char*, std::size_t> dequeueFromStream(size_t reqSize_);

    private:
        std::string            _stream;
        std::size_t            _startPos;
     };
}
#endif
        
 

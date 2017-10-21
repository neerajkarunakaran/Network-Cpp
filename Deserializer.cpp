#include <iostream>
#include "Deserializer.h"
#include <arpa/inet.h>
#include <cstdlib>
#include <string>
namespace Tcp
{
    Deserializer::Deserializer(std::size_t initialBufferSize_)
        : _stream()
        , _startPos(0)
    {
        _stream.reserve(initialBufferSize_);
    }

     void Deserializer::resetBuffer()
    {
        _stream.clear();
        _startPos = 0;
    }
    
    std::size_t Deserializer::getStreamSize() const
    {
        return _stream.size() - _startPos;
    }

    std::pair<bool, int> Deserializer::dequeueIntFromStream()
    {
        if(_startPos + sizeof(int) > _stream.size())
            return std::make_pair(false, int());
        
        int* p = reinterpret_cast<int*>(&(const_cast<char*>(_stream.c_str())[_startPos]));
        int changedInt = ntohl(*p);
        _startPos +=sizeof(int);
      
        return std::make_pair(true, changedInt);
    }
        

    std::pair<bool, std::string> Deserializer::dequeueStringFromStream(std::size_t size_)
    {
        if(_startPos + size_ > _stream.size())
            return std::make_pair(false, std::string());

        std::size_t curStartPos = _startPos;
        _startPos += size_;

        return std::make_pair(true, std::string(_stream, curStartPos, size_));
    }

    std::pair<bool, char> Deserializer::dequeueCharFromStream()
    {
        if(_startPos + sizeof(char)> _stream.size())
            return std::make_pair(false, char());

        std::size_t curStartPos = _startPos;
        _startPos += sizeof(char); 
        return std::make_pair(true ,_stream[curStartPos]);
    }

    void Deserializer::enqueuetoStream(const char* data_, std::size_t dataSize_)
    {
        _stream.append(data_, dataSize_ );
    }
   
}

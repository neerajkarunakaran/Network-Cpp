
#include <arpa/inet.h>
#include <utility>
#include "Serializer.h"
#include <cstring>

namespace Tcp
{
    Serializer::Serializer(std::size_t initialBufferSize_)
        : _stream()
        , _startPos(0)
    {
        _stream.reserve(initialBufferSize_);
    }
    
    void Serializer::resetBuffer()
    {
        _stream.clear();
        _startPos = 0;
    }
    
    std::size_t Serializer::getStreamSize() const
    {
        return _stream.size() - _startPos;
    }
    
    void Serializer::enqueueIntToStream(int value_)
    {
        int changedInt = htonl(value_);
        const char *buf = reinterpret_cast<char*>(&changedInt);
        _stream.append(buf, sizeof(int));
    }
    
    void Serializer::enqueueStringToStream(const std::string & string_)
    {
        _stream.append(string_);
        
    }
    
    void Serializer::enqueueCharToStream(char char_)
    {
        _stream.append(sizeof(char), char_);
    }

    std::pair<const char*, std::size_t> Serializer::dequeueFromStream(size_t reqSize_)
    {
        if(_startPos + reqSize_ > _stream.size())
            reqSize_ = _stream.size() - _startPos;

        std::size_t curStartPos = _startPos;
        _startPos += reqSize_;

        return std::make_pair(&_stream[curStartPos], reqSize_ );
    }
    
}


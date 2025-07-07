#pragma once
#include <stdexcept>
#include <string>

namespace TeaPacket::Error
{
    class NotImplemented : public std::logic_error
    {
    public:
        explicit NotImplemented(const std::string& message) : std::logic_error(message + " not implemented."){}
    };

    class FunctionNotImplemented : public NotImplemented
    {
    public:
        explicit FunctionNotImplemented(const std::string& functionName) : NotImplemented("Function " + functionName){}
    };
}

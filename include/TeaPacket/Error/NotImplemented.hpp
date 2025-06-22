#pragma once
#include <stdexcept>
#include <string>

namespace TeaPacket::Error
{
    class NotImplemented : public std::logic_error
    {
    private:
        std::string _message;
    public:
        explicit NotImplemented(const std::string& message) : _message(message){}
        char const * what() const noexcept override { return (_message + " not implemented.").c_str();}
    };

    class FunctionNotImplemented : public NotImplemented
    {
    private:
        std::string _functionName;
    public:
        explicit FunctionNotImplemented(const std::string& functionName) : _functionName(functionName){}
        char const * what() const override { return ("Function" + _functionName + " not implemented").c_str();}
    };
}

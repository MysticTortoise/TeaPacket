#pragma once
#include <stdexcept>
#include <string>

/// Custom errors specific to TeaPacket
namespace TeaPacket::Error
{
    /// Error for functionality that is not implemented yet.
    class NotImplemented : public std::logic_error
    {
    public:
        explicit NotImplemented(const std::string& message) : std::logic_error(message + " not implemented."){}
    };

    /// Error for a specific function that is not implemented yet. Essentially a stub to allow the function to exist, but error when calling it.
    class FunctionNotImplemented : public NotImplemented
    {
    public:
        explicit FunctionNotImplemented(const std::string& functionName) : NotImplemented("Function " + functionName){}
    };
}

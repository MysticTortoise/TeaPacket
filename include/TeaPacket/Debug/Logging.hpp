#pragma once

#include <string>

namespace TeaPacket
{
    void Print(const std::string& message);

    template<typename T>
    std::enable_if_t<std::is_arithmetic_v<T>> Print(const T& val)
    {
        Print(std::to_string(val));
    }

    template<>
    inline std::enable_if_t<std::is_arithmetic_v<char>> Print(const char& val)
    {
        Print(std::string(1, val));
    }

    template<typename T>
    std::enable_if_t<!std::is_arithmetic_v<T>> Print(const T& val)
    {
        Print(static_cast<std::string>(val));
    }

    inline void Print(const char* const& message)
    {
        Print(std::string(message));
    }

    inline void Print(char* const& message)
    {
        Print(std::string(message));
    }

    template<typename T>
    void PrintLine(const T& val)
    {
        Print(val);
        Print("\n");
    }
}
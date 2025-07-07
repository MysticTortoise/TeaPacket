#pragma once

#include <string>

namespace TeaPacket
{
    /// Prints a string to the debugging console.
    void Print(const std::string& message);

    /// Prints arithmetic values
    template<typename T>
    std::enable_if_t<std::is_arithmetic_v<T>> Print(const T& val)
    {
        Print(std::to_string(val));
    }

    /// Prints a character
    template<>
    inline std::enable_if_t<std::is_arithmetic_v<char>> Print(const char& val)
    {
        Print(std::string(1, val));
    }

    /// Prints a non-arithmetic, but string-convertable type.
    template<typename T>
    std::enable_if_t<!std::is_arithmetic_v<T>> Print(const T& val)
    {
        Print(static_cast<std::string>(val));
    }

    /// Prints a C string
    inline void Print(const char* const& message)
    {
        Print(std::string(message));
    }

    /// Also prints a C string.
    inline void Print(char* const& message)
    {
        Print(std::string(message));
    }

    /// Prints a value, followed by a line.
    template<typename T>
    void PrintLine(const T& val)
    {
        Print(val);
        Print("\n");
    }
}
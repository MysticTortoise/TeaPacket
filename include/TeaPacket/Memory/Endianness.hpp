#pragma once

#include <bit>
#include <cstdint>

namespace TeaPacket::Memory
{

    constexpr bool IsBigEndian()
    {
        return std::endian::native == std::endian::big;
    }

    constexpr uint16_t SwapEndian16(uint16_t v)
    {
#if defined(__GNUG__)
        return __builtin_bswap16(v);
#elif __cplusplus >= 202302L
        return std::byteswap(v);
#else
        return (v>>8) | (v<<8);
#endif
    }

    constexpr uint32_t SwapEndian32(uint32_t v)
    {
#if defined(__GNUG__)
        return __builtin_bswap32(v);
#elif __cplusplus >= 202302L
        return std::byteswap(v);
#else
        return ((v>>24)&0xff) | ((v<<8)&0xff0000) | ((v>>8)&0xff00) | ((v<<24)&0xff000000);
#endif
    }

    constexpr uint64_t SwapEndian64(uint64_t v)
    {
#if defined(__GNUG__)
            return __builtin_bswap64(v);
#elif __cplusplus >= 202302L
        return std::byteswap(v);
#else
            return SwapEndian32(v&0xFFFFFFFF) | SwapEndian32((v>>32)&0xFFFFFFFF);
#endif
    }


    template<typename T>
    T SwapEndian(T val) = delete;

    template<>
    inline uint16_t SwapEndian(uint16_t val)
    {
        return SwapEndian16(val);
    }

    template<>
    inline uint32_t SwapEndian(uint32_t val)
    {
        return SwapEndian32(val);
    }

    template<>
    inline uint64_t SwapEndian(uint64_t val)
    {
        return SwapEndian64(val);
    }

    template<>
    inline int16_t SwapEndian(int16_t val)
    {
        return std::bit_cast<int16_t, uint16_t>(SwapEndian16(std::bit_cast<uint16_t, int16_t>(val)));
    }

    template<>
    inline int32_t SwapEndian(int32_t val)
    {
        return std::bit_cast<int32_t, uint32_t>(SwapEndian32(std::bit_cast<uint32_t, int32_t>(val)));
    }

    template<>
    inline int64_t SwapEndian(int64_t val)
    {
        return std::bit_cast<int64_t, uint64_t>(SwapEndian64(std::bit_cast<uint64_t, int64_t>(val)));
    }

}

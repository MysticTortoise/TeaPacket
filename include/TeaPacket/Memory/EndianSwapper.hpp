#pragma once

#include <cstdint>
#include <stdlib.h>

namespace TeaPacket::Memory
{
    constexpr uint16_t SwapEndian16(uint16_t v)
    {
#if defined(__GNUG__)
        return __builtin_bswap16(v);
#elif defined(_MSC_VER)
        return _byteswap_ushort(v);
#else
        return (v>>8) | (v<<8);
#endif
    }

    constexpr uint32_t SwapEndian32(uint32_t v)
    {
#if defined(__GNUG__)
        return __builtin_bswap32(v);
#elif defined(_MSC_VER)
        return _byteswap_ulong(v);
#else
        return ((v>>24)&0xff) | ((v<<8)&0xff0000) | ((v>>8)&0xff00) | ((v<<24)&0xff000000);
#endif
    }

    constexpr uint64_t SwapEndian64(uint64_t v)
    {
#if defined(__GNUG__)
            return __builtin_bswap64(v);
#elif defined(_MSC_VER)
            return _byteswap_uint64(v);
#else
            return SwapEndian32(v&0xFFFFFFFF) | SwapEndian32((v>>32)&0xFFFFFFFF);
#endif
    }
        
}

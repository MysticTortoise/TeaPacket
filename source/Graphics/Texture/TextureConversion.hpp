#pragma once
#include "TeaPacket/Graphics/Texture.hpp"

namespace TeaPacket::Graphics
{
    void TEX_BPP24_TO_32(unsigned char** data, unsigned short width, unsigned short height);
    void TEX_SWAP_RANDB(unsigned char** data, unsigned short width, unsigned short height, TextureFormat format);

    TextureFormat Pl_TexConvertToNativeFormat(unsigned char** data, unsigned short width, unsigned short height, TextureFormat format);
}
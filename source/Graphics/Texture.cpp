#include "TeaPacket/Graphics/Texture.hpp"

#include <assert.h>

using namespace TeaPacket::Graphics;

void Texture::Initialize(const unsigned char* data)
{
    assert(data != nullptr);
    Pl_Initialize(data);
}

Texture Texture::CreateTexture(const unsigned char* data, unsigned short width, unsigned short height, TextureFilterType filterType, TextureWrapType wrapType)
{
    Texture texture;
    texture.width = width;
    texture.height = height;
    texture.wrapType = wrapType;
    texture.filterType = filterType;
    texture.Initialize(data);

    return texture;
}


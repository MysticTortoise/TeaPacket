#include "TeaPacket/Graphics/Texture.hpp"

#include <unordered_map>

using namespace TeaPacket::Graphics;

void Texture::Initialize(const unsigned char* data)
{
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

const static std::unordered_map<TextureFormat, TextureFormatInfo> formatInfo = {
    {TEXTURE_FORMAT_RGBA8, TextureFormatInfo(4, {8,8,8,8})},
    {TEXTURE_FORMAT_RGB8, TextureFormatInfo(3, {8,8,8})}
};

TextureFormatInfo Texture::GetFormatInfo(TextureFormat format)
{
    return formatInfo.at(format);
}

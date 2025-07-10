#pragma once

#include "Platform/Graphics/PlatformTexture.hpp"

namespace TeaPacket::Graphics
{
    enum TextureFilterType : unsigned char
    {
        TEXTURE_FILTER_POINT,
        TEXTURE_FILTER_LINEAR
    };

    enum TextureWrapType : unsigned char
    {
        TEXTURE_WRAP_REPEAT,
        TEXTURE_WRAP_MIRROR,
        TEXTURE_WRAP_CLAMP
    };


    class Texture
    {
    public:
        unsigned short width;
        unsigned short height;
        TextureFilterType filterType;
        TextureWrapType wrapType;


        PlatformTexture platformTexture;

        void Initialize(const unsigned char* data);
    private:
        void Pl_Initialize(const unsigned char* data);
    public:
        static Texture CreateTexture(const unsigned char* data, unsigned short width, unsigned short height, TextureFilterType filterType, TextureWrapType wrapType);
    private:
    };
}
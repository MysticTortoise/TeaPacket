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

        /// Initialize a texture.
        /// @param data The graphics data the texture needs. If nullptr, the texture's contents are uninitialized.
        void Initialize(const unsigned char* data);
    private:
        void Pl_Initialize(const unsigned char* data);
    public:
        /// Creates a texture from data.
        /// @param data The graphics data to be used. If nullptr, the texture's contents are uninitialized.
        /// @param width The width of the texture.
        /// @param height The height of the texture.
        /// @param filterType The filter type to be used by the texture.
        /// @param wrapType The wrap method used by the texture.
        static Texture CreateTexture(const unsigned char* data, unsigned short width, unsigned short height,
            TextureFilterType filterType = TEXTURE_FILTER_LINEAR, TextureWrapType wrapType = TEXTURE_WRAP_REPEAT);
    private:
    };
}
#pragma once

#include <vector>

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

    enum TextureFormat : unsigned short
    {
        TEXTURE_FORMAT_RGBA8,
        TEXTURE_FORMAT_RGB8,
    };


    class Texture
    {
    public:
        unsigned short width;
        unsigned short height;
        TextureFilterType filterType = TEXTURE_FILTER_LINEAR;
        TextureWrapType wrapType = TEXTURE_WRAP_REPEAT;
        TextureFormat format = TEXTURE_FORMAT_RGBA8;


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

        static constexpr std::vector<unsigned char> GetFormatChannelSizes(TextureFormat format){
            switch (format)
            {
            case TEXTURE_FORMAT_RGBA8: return {8,8,8,8};
            case TEXTURE_FORMAT_RGB8: return {8,8,8};
            }
            throw std::exception();
        }

    private:
    };
}
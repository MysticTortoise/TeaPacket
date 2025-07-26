#pragma once

#include <vector>

#include "Platform/Graphics/PlatformTexture.hpp"

namespace TeaPacket::Graphics
{
    enum class TextureFilterType : unsigned char
    {
        POINT,
        LINEAR
    };

    enum class TextureWrapType : unsigned char
    {
        REPEAT,
        MIRROR,
        CLAMP
    };

    enum class TextureFormat : unsigned short
    {
        RGBA8, ///< A 4 channel 8 bpp texture format that supports 8 channels for Red, Green, Blue, Alpha
        RGB8,
        BGRA8,
        BGR8,
        RGB5A1,
        BGR5A1,
        R8,
        NONE
    };


    class Texture
    {
    public:
        unsigned short width;
        unsigned short height;
        TextureFilterType filterType = TextureFilterType::LINEAR;
        TextureWrapType wrapType = TextureWrapType::REPEAT;
        TextureFormat format = TextureFormat::RGBA8;


        PlatformTexture platformTexture;

        /// Initialize a texture.
        /// @param data The graphics data the texture needs. If nullptr, the texture's contents are uninitialized.
        /// \warning The data will be modified and deleted when the function is done! If you want to keep the data,
        /// copy it before passing it to this method!!!
        void Initialize(unsigned char* data);
    private:
        void Pl_Initialize(unsigned char* data);
    public:

        static constexpr std::vector<unsigned char> GetFormatChannelSizes(TextureFormat format){
            using enum TextureFormat;
            switch (format)
            {
            case BGRA8:
            case RGBA8: return {8,8,8,8};
            case BGR8:
            case RGB8: return {8,8,8};
            case BGR5A1:
            case RGB5A1: return {5,5,5, 1};
            case R8: return {8};
            case NONE: throw std::exception();
            }
            throw std::exception();
        }

    private:
    };
}
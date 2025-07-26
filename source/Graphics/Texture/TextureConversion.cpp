#include "Graphics/Texture/TextureConversion.hpp"

#include <cstring>
#include <numeric>

#include "TeaPacket/Error/NotImplemented.hpp"
#include "TeaPacket/Graphics/Texture.hpp"

using namespace TeaPacket;
using namespace TeaPacket::Graphics;

/// Converts 24 bpp textures to 32bpp textures because FUCK MICROSOFT \n
/// Seriously they got rid of 24bpp textures in Direct3D 11 WHYYYYYYY
void Graphics::TEX_BPP24_TO_32(unsigned char** data, unsigned short width, unsigned short height)
{
    size_t src = 0;
    size_t dst = 0;
    unsigned char* newData = new unsigned char[width * height * 4];
    unsigned char* ogData = *data;
    for (unsigned short y = 0; y < height; y++)
    {
        for (unsigned short x = 0; x < width; x++)
        {
            memcpy(&newData[dst], &ogData[src], 3);
            newData[dst+3] = 255;
            dst += 4;
            src += 3;
        }
    }
    delete[] *data;
    *data = newData;
}

void Graphics::TEX_SWAP_RANDB(unsigned char** data, unsigned short width, unsigned short height, TextureFormat format)
{
    unsigned char* texData = *data;
    size_t i = 0;
    std::vector channelSizes = Texture::GetFormatChannelSizes(format);
    unsigned char bytesPerPixel = static_cast<unsigned char>(std::accumulate(channelSizes.begin(), channelSizes.end(), 0)) / 8;
    bool byteAligned = true;

    for (const auto channel : channelSizes)
    {
        if (channel % 8 != 0)
        {
            byteAligned = false;
        }
    }


    if (byteAligned)
    {
        const unsigned char bytesPerChannel = channelSizes[0]/8;
        unsigned char* temp = new unsigned char[bytesPerChannel];

        unsigned char bOffset = bytesPerChannel +  channelSizes[1]/8;

        for (unsigned short y = 0; y < height; y++)
        {
            for (unsigned short x = 0; x < width; x++)
            {
                memcpy(temp, &texData[i], bytesPerChannel);
                memcpy(&texData[i], &texData[i+bOffset], bytesPerChannel);
                memcpy(&texData[i+bOffset], temp, bytesPerChannel);
                i += bytesPerPixel;
            }
        }
        delete[] temp;
    } else
    {
        unsigned char b1;
        unsigned char b2;
        switch (format)
        {
            using enum TextureFormat;
        case BGR5A1:
        case RGB5A1:
            for (unsigned short y = 0; y < height; y++)
            {
                for (unsigned short x = 0; x < width; x++)
                {
                    b1 = texData[i];
                    b2 = texData[i+1];
                    texData[i++] = (b2 & 0b00111110) << 2 | b1 & 0b00000111;
                    texData[i++] = (b1 & 0b11111000) >> 2 | b2 & 0b11000001;
                }
            }
            break;
        default:
            throw Error::NotImplemented("Format doesn't have a proper SWAP_RANDB function");
        }
    }
}
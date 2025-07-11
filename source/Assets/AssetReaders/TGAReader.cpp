#include "TeaPacket/Assets/AssetReaders/TGAReader.hpp"
#include "TeaPacket/Assets/AssetStream.hpp"
#include "TeaPacket/Debug/Logging.hpp"
#include "TeaPacket/Error/NotImplemented.hpp"

using namespace TeaPacket::Assets;
using namespace TeaPacket::Graphics;

Texture TeaPacket::Assets::ReadTGA(const std::string& path)
{
    AssetStream stream(path);
    Texture tex;
    // Parse Header
    stream.Skip(1); // unsigned char idLen = stream.ReadByte();
    const unsigned char colorMap = stream.ReadByte();
    if (colorMap != 0)
    {
        throw Error::NotImplemented("Colormapped TGA files not yet supported.");
    }
    const unsigned char imgType = stream.ReadByte();
    if (imgType != 2)
    {
        throw Error::NotImplemented("Only uncompressed true-color TGA files are supported.");
    }
    stream.Skip(5); // Not yet implemented. - Color map
    stream.Skip(4); // Origin not important(?)

    tex.width = stream.ReadUInt16LittleEndian();
    tex.height = stream.ReadUInt16LittleEndian();
    const unsigned char bpp = stream.ReadByte();
    const unsigned char bytesPerPixel = bpp / 8;
    const unsigned char imgDesc = stream.ReadByte();
    const unsigned char alphaChannelDepth = imgDesc & 0b00001111;
    const unsigned char pixelOrder = imgDesc & 0b00110000;

    if (bpp == 24)
    {
        tex.format = TEXTURE_FORMAT_RGB8;
    } else
    {
        tex.format = TEXTURE_FORMAT_RGBA8;
        if (alphaChannelDepth != bpp / 4)
        {
            throw Error::NotImplemented("Can't comprehend TGA format.");
        }
    }

    bool rightToLeft = pixelOrder & 0b00010000;
    bool topTobottom = pixelOrder & 0b00100000;
    if (rightToLeft | topTobottom){}

    size_t imgDataSize = tex.width * tex.height * bytesPerPixel;
    unsigned char* data = new unsigned char[imgDataSize];

    if (topTobottom)
    {
    }
    else
    {
        for (int y = tex.height-1; y > 0; --y)
        {
            if (rightToLeft)
            {
            }
            else
            {
                int rowStart = y * tex.width * bytesPerPixel;
                stream.ReadBytes(tex.width * bytesPerPixel, reinterpret_cast<char*>(&data[rowStart]));
            }
        }
    }
    for (unsigned long i = 0; i < imgDataSize; i += bytesPerPixel)
    {
        std::swap(data[i], data[i+2]);
    }


    //stream.ReadBytes(imgDataSize, reinterpret_cast<char*>(data));

    tex.Initialize(data);

    delete[] data;

    return tex;
}

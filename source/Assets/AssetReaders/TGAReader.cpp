#include "TeaPacket/Assets/AssetReaders/TGAReader.hpp"
#include "TeaPacket/Assets/AssetStream.hpp"
#include "TeaPacket/Debug/Logging.hpp"
#include "TeaPacket/Error/NotImplemented.hpp"

#include <cstdint>

using namespace TeaPacket::Assets;
using namespace TeaPacket::Graphics;


enum class TGAImageType : uint8_t
{
    NO_IMAGE = 0,
    UNCOMPRESSED_COLORMAPPED = 1,
    UNCOMPRESSED_TRUECOLOR = 2,
    UNCOMPRESSED_GRAYSCALE = 3,
    RLE_COLORMAPPED = 9,
    RLE_TRUCOLOR = 10,
    RLE_GRAYSCALE = 11
};

enum class TGAAttributeType : uint8_t
{
    NO_ALPHA = 0,
    UNIMPORTANT_ALPHA = 1,
    UNIMPORTANT_ALPHA_BUT_NOT_REALLY = 2,
    USEFUL_ALPHA = 3,
    PREMULTIPLIED_ALPHA = 4,
};

struct TGAInfo{
    TGAImageType type;
    unsigned short width, height;
    TextureFormat format;
    unsigned char bpp;
    unsigned char bytesPerPixel;

    bool rightToLeft, topToBottom;

    struct {
        uint16_t firstColorIndex;
        uint16_t mapLength;
        uint8_t entryBPP;
    } colorMapInfo;
};

static constexpr TextureFormat GetFormatFromBPP(unsigned char bpp)
{
    switch (bpp)
    {
    case 8: return TextureFormat::R8;
    case 16: return TextureFormat::BGR5A1;
    case 24: return TextureFormat::BGR8;
    case 32: return TextureFormat::BGRA8;
    default: throw std::invalid_argument("Invalid or unsupported bpp");
    }
}



using namespace TeaPacket;

static void ReadHeader(AssetStream& stream, TGAInfo& info)
{
    using enum TGAImageType;

    const unsigned char idLen = stream.ReadByte();
    stream.Skip(1);

    info.type = static_cast<TGAImageType>(stream.ReadUInt8());
    info.colorMapInfo.firstColorIndex = stream.ReadUInt16LittleEndian();
    info.colorMapInfo.mapLength = stream.ReadUInt16LittleEndian();
    info.colorMapInfo.entryBPP = static_cast<uint8_t>(stream.ReadByte());
    stream.Skip(4); // Origin not important(?)

    info.width = stream.ReadUInt16LittleEndian();
    info.height = stream.ReadUInt16LittleEndian();
    info.bpp = stream.ReadByte();
    const unsigned char imgDesc = stream.ReadByte();
    //const unsigned char alphaChannelDepth = imgDesc & 0b00001111;
    const unsigned char pixelOrder = imgDesc & 0b00110000;

    // Set up Texture properties
    info.format = GetFormatFromBPP(info.bpp);
    info.rightToLeft = pixelOrder & 0b00010000;
    info.topToBottom = pixelOrder & 0b00100000;
    // Skip Image ID field
    stream.Skip(idLen);
}

static void ReadImageNoPallette(AssetStream& stream, const TGAInfo& info, unsigned char** pData, size_t* imgDataSize)
{
    *imgDataSize = info.width * info.height * info.bytesPerPixel;\
    *pData = new unsigned char [*imgDataSize];
    unsigned char* data = *pData;

    if (info.topToBottom)
    {
    }
    else
    {
        for (int y = info.height-1; y >= 0; --y)
        {
            if (info.rightToLeft)
            {
            }
            else
            {
                int rowStart = y * info.width * info.bytesPerPixel;
                stream.ReadBytes(info.width * info.bytesPerPixel, reinterpret_cast<char*>(&data[rowStart]));
            }
        }
    }
}

static void ReadImagePalletted(AssetStream& stream, TGAInfo& info, unsigned char** pData, size_t* imgDataSize)
{
    if (info.bpp != 8)
    {
        throw Error::NotImplemented("Paletted TGA files with a BPP other than 8 are not supported.");
    }

    info.format = GetFormatFromBPP(info.colorMapInfo.entryBPP);
    unsigned char entryBytesPerPixel = info.colorMapInfo.entryBPP / 8;

    // Read colormap
    unsigned char* colorMap = new unsigned char[info.colorMapInfo.mapLength * entryBytesPerPixel];
    stream.ReadBytes(info.colorMapInfo.mapLength * entryBytesPerPixel, reinterpret_cast<char*>(colorMap));

    // Allocate image data
    *imgDataSize = info.width * info.height * entryBytesPerPixel;
    *pData = new unsigned char [*imgDataSize];
    unsigned char* data = *pData;

    unsigned int i = 0;
    for (unsigned short y = 0; y < info.height; y++)
    {
        for (unsigned short x = 0; x < info.width; x++)
        {
            memcpy(&data[i], &colorMap[stream.ReadUInt8() * entryBytesPerPixel], entryBytesPerPixel);
            i += entryBytesPerPixel;
        }
    }

    delete[] colorMap;
}

static void FixAlpha(){}

static void ReadFooter(AssetStream& stream, TGAInfo& info, unsigned char* data, const size_t imgDataSize)
{
    using enum TGAAttributeType;
    // Read TGA footer
    stream.Seek(-26, std::ios::end);
    const uint32_t extensionOffset = stream.ReadUInt32LittleEndian();
    const uint32_t devAreaOffset = stream.ReadUInt32LittleEndian();
    char signature[17];
    stream.ReadBytes(sizeof(signature), signature);
    if (!std::memcmp(signature, "TRUEVISION-XFILE.", sizeof(signature)))
    {
        // TGA Footer exists!
        if (extensionOffset != 0)
        {
            stream.Seek(extensionOffset, std::ios::beg);
            (void)devAreaOffset;
            if (stream.ReadUInt16LittleEndian() == 495)
            {
                stream.Skip(41 + 324 + 12 + 41 + 6 + 41 + 3); // Useless metadata.
                stream.Skip(4 + 4 + 4 + 4 + 4 + 4); // Unsupported data
                TGAAttributeType attribute = static_cast<TGAAttributeType>(stream.ReadByte());
                if (attribute != USEFUL_ALPHA && attribute != NO_ALPHA || (attribute == NO_ALPHA && info.format == TextureFormat::BGR5A1))
                {
                    switch (info.format)
                    {
                    case TextureFormat::BGRA8:
                        for (unsigned long i = 0; i < imgDataSize; i+=4)
                        {
                            data[i+3] = 255;
                        }
                        break;
                    case TextureFormat::BGR5A1:
                        for (unsigned long i = 0; i < imgDataSize; i+=2)
                        {
                            data[i+1] |= 0b10000000;
                        }
                        break;
                    default:
                        break;
                    }
                }
            }
        }
    }
}



Texture Assets::ReadTGA(const std::string& path)
{

    AssetStream stream(path);
    Texture tex;
    TGAInfo info;

    ReadHeader(stream, info);

    info.bytesPerPixel = info.bpp / 8;
    size_t imgDataSize = 0;
    unsigned char* data = nullptr;

    switch (info.type)
    {
        using enum TGAImageType;
    case UNCOMPRESSED_TRUECOLOR:
    case UNCOMPRESSED_GRAYSCALE:
        ReadImageNoPallette(stream, info, &data, &imgDataSize); break;
    case UNCOMPRESSED_COLORMAPPED:
        ReadImagePalletted(stream, info, &data, &imgDataSize); break;

    default: break;
    }
    ReadFooter(stream, info, data, imgDataSize);

    tex.width = info.width;
    tex.height = info.height;
    tex.format = info.format;

    tex.Initialize(data);
    return tex;
}

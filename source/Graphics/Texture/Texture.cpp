#include "TeaPacket/Graphics/Texture.hpp"
#include "Graphics/Texture/TextureConversion.hpp"

using namespace TeaPacket::Graphics;

void Texture::Initialize(unsigned char* data)
{
    format = Pl_TexConvertToNativeFormat(&data, width, height, format);
    Pl_Initialize(data);
    delete[] data;
}
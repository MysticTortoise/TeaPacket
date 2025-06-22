#include "TeaPacket/Graphics/Viewport.hpp"

using namespace TeaPacket::Graphics;

Viewport::Viewport(unsigned short width, unsigned short height):
width(width),
height(height)
{}

void Viewport::DeInitialize()
{
    Pl_DeInitialize();
}



Viewport Viewport::CreateViewport(const unsigned short width, const unsigned short height, Display* display)
{
    return Pl_CreateViewport(width, height, display);
}

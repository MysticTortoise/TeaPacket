#include "TeaPacket/Graphics/Viewport.hpp"

#include "TeaPacket/Graphics/Display.hpp"

using namespace TeaPacket::Graphics;

Viewport::Viewport(unsigned short width, unsigned short height):
width(width),
height(height)
{}

void Viewport::Initialize()
{
    Pl_Initialize();
}


void Viewport::DeInitialize()
{
    Pl_DeInitialize();
}

Viewport Viewport::CreateViewport(const unsigned short width, const unsigned short height)
{
    Viewport viewport = Viewport(width, height);
    viewport.Initialize();
    return viewport;
}

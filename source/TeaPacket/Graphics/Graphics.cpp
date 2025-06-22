#include "TeaPacket/Graphics/Graphics.hpp"
#include "TeaPacket/Graphics/Pl_Graphics.hpp"

using namespace TeaPacket;

void Graphics::Initialize()
{
    Pl_Initialize();
    initialized = true;
}

void Graphics::DeInitialize()
{
    Pl_DeInitialize();
    initialized = false;
}

void Graphics::BeginRenderToViewport(Viewport& viewport)
{
    currentViewport = &viewport;
    Pl_BeginRenderToViewport(viewport);
}

void Graphics::FinishRenderToViewport()
{
    Pl_FinishRenderToViewport();
    currentViewport = nullptr;
}

void Graphics::ClearScreen(const Color4& color)
{
    Pl_ClearScreen(color);
}

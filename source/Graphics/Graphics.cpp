#include "TeaPacket/Graphics/Graphics.hpp"
#include "Graphics/Pl_Graphics.hpp"

using namespace TeaPacket;

bool Graphics::Initialize()
{
    if (!Pl_Initialize()) { return false; }
    initialized = true;
    return true;
}

void Graphics::DeInitialize()
{
    Pl_DeInitialize();
    initialized = false;
}

void Graphics::SetRenderTarget(Viewport& viewport)
{
    currentViewport = &viewport;
    Pl_BeginRenderTarget(viewport);
}

void Graphics::SetRenderTarget(Display& display)
{
    SetRenderTarget(display.viewport);
}

void Graphics::FinishRenderTarget()
{
    Pl_FinishRenderTarget();
    currentViewport = nullptr;
}


void Graphics::ClearScreen(const Color4& color)
{
    Pl_ClearScreen(color);
}

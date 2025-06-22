//
// Windows Graphics functions implementation.
//
#include "TeaPacket/Graphics/Graphics.hpp"
#include "TeaPacket/Graphics/Pl_Graphics.hpp"
#include "TeaPacket/Graphics/Display.hpp"

#include <d3dx9math.h>

using namespace TeaPacket;

void Graphics::Pl_Initialize()
{
    Display::RegisterDisplay(1280, 720, "TeaPacket");
}

void Graphics::Pl_DeInitialize()
{
    for (Display& display : Display::Displays)
    {
        display.DeInitialize();
    }
}

void Graphics::Pl_BeginRenderToViewport(Viewport& viewport)
{
    (void)viewport;
}

void Graphics::Pl_FinishRenderToViewport()
{
    platformDisplay.swapchain->Present(0, 0);
}

void Graphics::Pl_ClearScreen(const Color4& color)
{
    currentViewport->platformDisplay.deviceContext->ClearRenderTargetView(currentViewport->platformDisplay.backbuffer,
        D3DXCOLOR(color.r() / 255.0f, color.g() / 255.0f, color.b() / 255.0f, color.a()));
}


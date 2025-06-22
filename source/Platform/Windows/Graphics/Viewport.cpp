#include "TeaPacket/Graphics/Viewport.hpp"
#include "TeaPacket/Graphics/Display.hpp"
#include "TeaPacket/Error/NotImplemented.hpp"

using namespace TeaPacket::Graphics;


Viewport Viewport::Pl_CreateViewport(unsigned short width, unsigned short height, Display* display)
{
    if (display == nullptr)
    {
        throw Error::NotImplemented("Viewports separate from Displays");
    }
    Viewport viewport = Viewport(width, height);

    const Platform::WindowsDisplay& platformData = display.platformDisplay;
    ID3D11Texture2D* textureBackbuffer;
    platformData.swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<LPVOID*>(&textureBackbuffer));
    platformData.device->CreateRenderTargetView(textureBackbuffer, NULL, &viewport.platformViewport.renderTargetView);
    textureBackbuffer->Release();
    display->platformDisplay.deviceContext->OMSetRenderTargets(1, &viewport.platformViewport.renderTargetView, NULL);
}

void Viewport::Pl_DeInitialize()
{
    platformViewport.renderTargetView->Release();
}

//
// Windows Graphics functions implementation.
//
#include "TeaPacket/Graphics/Graphics.hpp"
#include "Graphics/Pl_Graphics.hpp"
#include "TeaPacket/Graphics/Display.hpp"
#include "TeaPacket/Error/NotImplemented.hpp"
#include "Error/Win32ErrorHandling.hpp"

#include "d3d11.h"
#include "wrl/client.h"
#include <comdef.h>
#include <array>

#include "TeaPacket/Debug/Logging.hpp"
#include "TeaPacket/Graphics/Color4.hpp"
#include "TeaPacket/Memory/StructUtils.hpp"

using namespace TeaPacket;

namespace TeaPacket::Graphics
{
    Microsoft::WRL::ComPtr<ID3D11Device> device;
    Microsoft::WRL::ComPtr<ID3D11DeviceContext> deviceContext;
}

static D3D_FEATURE_LEVEL d3dFeatureLevels[] = {
    D3D_FEATURE_LEVEL_11_1,
    D3D_FEATURE_LEVEL_11_0,
    D3D_FEATURE_LEVEL_10_1,
    D3D_FEATURE_LEVEL_10_0,
    D3D_FEATURE_LEVEL_9_3,
    D3D_FEATURE_LEVEL_9_2,
    D3D_FEATURE_LEVEL_9_1,
};

bool Graphics::Pl_Initialize()
{
    UINT deviceFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
#ifdef TP_DEBUG
    deviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

    Microsoft::WRL::ComPtr<ID3D11Device> _device;
    Microsoft::WRL::ComPtr<ID3D11DeviceContext> _deviceContext;

    HRESULT result = D3D11CreateDevice(
        nullptr,
        D3D_DRIVER_TYPE_HARDWARE,
        0,
        deviceFlags,
        d3dFeatureLevels,
        ARRAYSIZE(d3dFeatureLevels),
        D3D11_SDK_VERSION,
        &_device,
        NULL, // Can be used later to determine the supported feature level.
        &_deviceContext
        );

    CheckErrorWinCom(result);

    _device.As(&device);
    _deviceContext.As(&deviceContext);

    const Display* display = Display::RegisterDisplay(1280, 720, "TeaPacket");

    return display == nullptr;
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
    deviceContext->OMSetRenderTargets(1,
        viewport.platformViewport.renderTargetView.GetAddressOf(),
        viewport.platformViewport.depthStencilView.Get());

    deviceContext->RSSetViewports(1, &viewport.platformViewport.d3dviewport);


    // DEBUG
    float col[4] = {1.0f, 0.0f, 0.0f, 1.0f};
    deviceContext->ClearRenderTargetView(viewport.platformViewport.renderTargetView.Get(), col);
    deviceContext->ClearDepthStencilView(viewport.platformViewport.depthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void Graphics::Pl_FinishRenderToViewport()
{

}

void Graphics::Pl_RenderViewportToDisplay(Display& display, Viewport& viewport)
{
    deviceContext->OMSetRenderTargets(1,
            display.platformDisplay.renderTargetView.GetAddressOf(),
            display.platformDisplay.depthStencilView.Get());

    deviceContext->RSSetViewports(1, &display.platformDisplay.d3dviewport);

    float colorArray[4] = {1, 0, 0, 1};
    deviceContext->ClearRenderTargetView(display.platformDisplay.renderTargetView.Get(), colorArray);

    display.platformDisplay.swapchain->Present(0, 0);
    (void)viewport;
}


void Graphics::Pl_ClearScreen(const Color4& color)
{
    float colorArray[4] = {color.r() / 255.0f, color.g() / 255.0f, color.b() / 255.0f, color.a() / 255.0f};
    deviceContext->ClearRenderTargetView(currentViewport->platformViewport.renderTargetView.Get(), colorArray);
}


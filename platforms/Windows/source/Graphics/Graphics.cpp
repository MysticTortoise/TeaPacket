//
// Windows Graphics functions implementation.
//
#include "TeaPacket/Graphics/Graphics.hpp"
#include "TeaPacket/Graphics/Pl_Graphics.hpp"
#include "TeaPacket/Graphics/Display.hpp"
#include "TeaPacket/Error/NotImplemented.hpp"

#include "d3d11.h"
#include "wrl/client.h"
#include <comdef.h>
#include <d3dx9math.h>

using namespace TeaPacket;

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
    const Display* display = Display::RegisterDisplay(1280, 720, "TeaPacket");

    UINT deviceFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
#ifdef TP_DEBUG
    deviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

    Microsoft::WRL::ComPtr<ID3D11Device> device;
    Microsoft::WRL::ComPtr<ID3D11DeviceContext> deviceContext;

    HRESULT result = D3D11CreateDevice(
        nullptr,
        D3D_DRIVER_TYPE_HARDWARE,
        0,
        deviceFlags,
        d3dFeatureLevels,
        ARRAYSIZE(d3dFeatureLevels),
        D3D11_SDK_VERSION,
        &device,
        NULL, // Can be used later to determine the supported feature level.
        &deviceContext
        );

    if (FAILED(result))
    {
        const _com_error err(result);
        const LPCTSTR errorMessage = err.ErrorMessage();
        throw std::runtime_error(errorMessage);
    }

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
    (void)viewport;
}

void Graphics::Pl_FinishRenderToViewport()
{

}

void Graphics::Pl_ClearScreen(const Color4& color)
{
    (void)color;
}


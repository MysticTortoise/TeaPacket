//
// Display implementation file for Windows using Win32
//
#include "../Interface/WindowsInterface.hpp"
#include "TeaPacket/Graphics/Display.hpp"
#include "Graphics/WindowsGraphics.hpp"
#include "Error/Win32ErrorHandling.hpp"
#include "TeaPacket/Math/Vector2.hpp"

#include "TeaPacket/Graphics/Model.hpp"
#include "TeaPacket/Graphics/Shader.hpp"
#include "TeaPacket/Graphics/Texture.hpp"

#include <windows.h>
#include <iostream>
#include <wrl/client.h>
#include <dxgi1_3.h>

#include "TeaPacket/Debug/Logging.hpp"
#include "TeaPacket/Memory/StructUtils.hpp"


using namespace TeaPacket::Graphics;

static Vector2l GetFullDesktopResolution()
{
    RECT desktop;
    GetWindowRect(GetDesktopWindow(), &desktop);
    return Vector2l{desktop.right, desktop.bottom};
}

static Vector2l GetDesktopResolution()
{
    return Vector2l{GetSystemMetrics(SM_CXFULLSCREEN), GetSystemMetrics(SM_CYFULLSCREEN)};
}

Display Display::Pl_RegisterDisplay(const unsigned short width, const unsigned short height, const std::string& name, bool* success)
{
    Display display = Display(width, height, name);

    RECT rect;
    Vector2l screenSize = {1280, 720};
    SetRect(&rect, 0, 0, screenSize.x(), screenSize.y());
    //AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false);
    display.platformDisplay.windowHandle = CreateWindow(
        "TeaPacket_MainWindowClass",
        name.c_str(),
        WS_OVERLAPPEDWINDOW,
        0, 0,
        rect.right - rect.left, rect.bottom - rect.top,
        0,
        0,
        Interface::Platform::instanceHandle,
        0
    );

    if (display.platformDisplay.windowHandle == NULL)
    {
        if (DWORD dwError = GetLastError(); dwError != ERROR_CLASS_ALREADY_EXISTS)
        {
            CheckErrorWinCom(HRESULT_FROM_WIN32(dwError));
        }
    }
    ShowWindow(display.platformDisplay.windowHandle, SW_SHOWNORMAL);

    DXGI_SWAP_CHAIN_DESC desc;
    ZeroStruct(desc);
    desc.Windowed = TRUE;
    desc.BufferCount = 2;
    desc.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
    desc.BufferDesc.Width = rect.right - rect.left;
    desc.BufferDesc.Height = rect.bottom - rect.top;
    desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    desc.SampleDesc.Count = 1;
    desc.SampleDesc.Quality = 0;
    desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
    desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
    desc.OutputWindow = display.platformDisplay.windowHandle;
    desc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

    Microsoft::WRL::ComPtr<IDXGIDevice3> dxgiDevice;
    CheckErrorWinCom(device.As(&dxgiDevice));

    Microsoft::WRL::ComPtr<IDXGIAdapter> adapter;
    Microsoft::WRL::ComPtr<IDXGIFactory> factory;

    CheckErrorWinCom(dxgiDevice->GetAdapter(&adapter));
    CheckErrorWinCom(adapter->GetParent(IID_PPV_ARGS(&factory)));
    CheckErrorWinCom(
        factory->CreateSwapChain(device.Get(), &desc, &display.platformDisplay.swapchain)
        );

    Microsoft::WRL::ComPtr<ID3D11Texture2D> backbuffer;
    CheckErrorWinCom(
        display.platformDisplay.swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<LPVOID*>(backbuffer.GetAddressOf()))
    );

    display.viewport.texture.platformTexture.texture2d = backbuffer.Get();


    CheckErrorWinCom(
        device->CreateRenderTargetView(
            backbuffer.Get(),
            NULL, display.viewport.platformViewport.renderTargetView.ReleaseAndGetAddressOf())
        );

    D3D11_TEXTURE2D_DESC depthBufferDesc;
    ZeroStruct(depthBufferDesc);
    depthBufferDesc.Width = width;
    depthBufferDesc.Height = height;
    depthBufferDesc.MipLevels = 1;
    depthBufferDesc.ArraySize = 1;
    depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    depthBufferDesc.SampleDesc.Count = 1;
    depthBufferDesc.SampleDesc.Quality = 0;
    depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    depthBufferDesc.CPUAccessFlags = 0;
    depthBufferDesc.MiscFlags = 0;

    CheckErrorWinCom(
        device->CreateTexture2D(&depthBufferDesc, NULL, display.viewport.platformViewport.depthTexture.ReleaseAndGetAddressOf())
    );

    D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
    ZeroStruct(depthStencilDesc);

    depthStencilDesc.DepthEnable = true;
    depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

    depthStencilDesc.StencilEnable = true;
    depthStencilDesc.StencilReadMask = 0xFF;
    depthStencilDesc.StencilWriteMask = 0xFF;

    depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
    depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
    depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
    depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
    depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
    depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
    depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
    depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

    CheckErrorWinCom(
        device->CreateDepthStencilState(&depthStencilDesc, display.platformDisplay.depthStencilState.GetAddressOf())
    );
    deviceContext->OMSetDepthStencilState(display.platformDisplay.depthStencilState.Get(), 1);

    D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
    ZeroStruct(depthStencilViewDesc);

    depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
    depthStencilViewDesc.Texture2D.MipSlice = 0;

    CheckErrorWinCom(
        device->CreateDepthStencilView(\
            display.viewport.platformViewport.depthTexture.Get(),
            &depthStencilViewDesc,
            display.viewport.platformViewport.depthStencilView.ReleaseAndGetAddressOf())
    );

    display.viewport.platformViewport.d3dviewport.Width = width;
    display.viewport.platformViewport.d3dviewport.Height = height;
    display.viewport.platformViewport.d3dviewport.MinDepth = 0;
    display.viewport.platformViewport.d3dviewport.MaxDepth = 1;
    display.viewport.platformViewport.d3dviewport.TopLeftX = 0;
    display.viewport.platformViewport.d3dviewport.TopLeftY = 0;

    *success = true;
    return display; 
}

bool Display::Pl_Resize(const unsigned short newWidth, const unsigned short newHeight)
{
    (void)newWidth;
    (void)newHeight;
    return true;
}

void Display::Pl_PresentDisplay()
{
    platformDisplay.swapchain->Present(0,0);
}



void Display::Pl_DeInitialize()
{
    platformDisplay.swapchain->SetFullscreenState(FALSE, NULL);
}
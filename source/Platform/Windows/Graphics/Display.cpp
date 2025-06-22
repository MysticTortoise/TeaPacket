//
// Display implementation file for Windows using Win32
//
#include "../Interface/PlatformInterface.hpp"
#include "TeaPacket/Graphics/Display.hpp"

#include "windows.h"

#include <iostream>

using namespace TeaPacket::Graphics;

Display Display::Pl_RegisterDisplay(const unsigned short width, const unsigned short height, const std::string& name, bool* success)
{
    Display display = Display(width, height, name);

    // find the appropriate window size for the given resolution
    RECT rect ={0, 0, width, height};
    AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW,FALSE);

    // Create the window itself.
    const HWND hWnd = CreateWindowEx(
        0,
        "MainWindowClass",
        name.c_str(),
        WS_OVERLAPPEDWINDOW,
        25,
        25,
        rect.right - rect.left,
        rect.bottom - rect.top,
        NULL,
        NULL,
        Interface::Platform::hInstance,
        NULL
    );
    // Show the window
    ShowWindow(hWnd, SW_SHOWNORMAL);

    // Add the window handle to the Display's private data.
    display.platformDisplay.windowHandle = hWnd;

    // Create the Swapchain
    DXGI_SWAP_CHAIN_DESC scd;
    ZeroMemory(&scd, sizeof(scd));

    scd.BufferCount = 1;
    scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    scd.OutputWindow = display.platformDisplay.windowHandle;
    scd.SampleDesc.Count = 4;
    scd.Windowed = TRUE;

    D3D11CreateDeviceAndSwapChain(
        NULL,
        D3D_DRIVER_TYPE_HARDWARE,
        NULL,
        0,
        NULL,
        0,
        D3D11_SDK_VERSION,
        &scd,
        &display.platformDisplay.swapchain,
        &display.platformDisplay.device,
        NULL,
        &display.platformDisplay.deviceContext
        );

    D3D11_VIEWPORT viewport;
    ZeroMemory(&viewport, sizeof(viewport));

    viewport.TopLeftX = 0;
    viewport.TopLeftY = 0;
    viewport.Width = display.width;
    viewport.Height = display.height;

    display.platformDisplay.deviceContext->RSSetViewports(1, &viewport);

    *success = true;
    return display;
}

void Display::Pl_DeInitialize() const
{
    platformDisplay.swapchain->Release();
    platformDisplay.device->Release();
    platformDisplay.deviceContext->Release();
}

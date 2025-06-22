#pragma once
#include "windows.h"
#include <d3d11.h>

namespace TeaPacket::Graphics
{
    namespace Platform
    {
        /// Platform-Specific display data for Windows.
        class WindowsDisplay
        {
        public:
            HWND windowHandle; ///< The handle to this Display's window.
            IDXGISwapChain* swapchain; ///< The swapchain used by this window's Graphics.
            ID3D11Device* device; ///< The Direct3D11 device this window uses.
            ID3D11DeviceContext* deviceContext; ///< The Direct3D11 device context this window uses.
        };
    }
    typedef Platform::WindowsDisplay PlatformDisplay;
}

#pragma once
#include <wrl/client.h>
#include <dxgi.h>

namespace TeaPacket::Graphics
{
    namespace Platform
    {
        /// Platform-Specific display data for Windows.
        class WindowsDisplay
        {
        public:
            HWND windowHandle;
            Microsoft::WRL::ComPtr<IDXGISwapChain> swapchain;
        };
    }
    typedef Platform::WindowsDisplay PlatformDisplay;
}

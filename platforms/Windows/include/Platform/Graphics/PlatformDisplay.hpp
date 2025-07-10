#pragma once
#include <d3d11.h>
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
            Microsoft::WRL::ComPtr<ID3D11DepthStencilState> depthStencilState;
        };
    }
    typedef Platform::WindowsDisplay PlatformDisplay;
}

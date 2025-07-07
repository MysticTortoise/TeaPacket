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
            Microsoft::WRL::ComPtr<ID3D11RenderTargetView> renderTargetView;
            Microsoft::WRL::ComPtr<ID3D11Texture2D> depthStencilBuffer;
            Microsoft::WRL::ComPtr<ID3D11DepthStencilState> depthStencilState;
            Microsoft::WRL::ComPtr<ID3D11DepthStencilView> depthStencilView;

            D3D11_VIEWPORT d3dviewport;
        };
    }
    typedef Platform::WindowsDisplay PlatformDisplay;
}

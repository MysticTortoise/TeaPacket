#pragma once

#include <d3d11.h>
#include <wrl/client.h>

#include <iostream>

namespace TeaPacket::Graphics
{
    namespace Platform
    {
        class WindowsViewport
        {
        public:
            Microsoft::WRL::ComPtr<ID3D11RenderTargetView> renderTargetView;

            Microsoft::WRL::ComPtr<ID3D11Texture2D> depthTexture;
            Microsoft::WRL::ComPtr<ID3D11DepthStencilView> depthStencilView;

            D3D11_VIEWPORT d3dviewport;
        };
    }

    typedef Platform::WindowsViewport PlatformViewport;

}

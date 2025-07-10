#pragma once

#include <d3d11.h>
#include <wrl/client.h>

namespace TeaPacket::Graphics
{
    namespace Platform
    {
        /// Platform-Specific display data for Windows.
        class WindowsTexture
        {
        public:
            Microsoft::WRL::ComPtr<ID3D11SamplerState> samplerState;
            Microsoft::WRL::ComPtr<ID3D11Texture2D> texture2d;
            Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> textureView;
        };
    }
    typedef Platform::WindowsTexture PlatformTexture;
}

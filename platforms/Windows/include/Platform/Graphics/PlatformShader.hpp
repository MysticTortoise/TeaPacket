#pragma once

#include <d3d11.h>
#include <wrl/client.h>

namespace TeaPacket::Graphics
{
    namespace Platform
    {
        /// Platform-Specific display data for Windows.
        class WindowsShader
        {
        public:
            Microsoft::WRL::ComPtr<ID3D11VertexShader> vertexShader;
            Microsoft::WRL::ComPtr<ID3D11PixelShader> pixelShader;
            Microsoft::WRL::ComPtr<ID3D11InputLayout> inputLayout;
        };
    }
    typedef Platform::WindowsShader PlatformShader;
}

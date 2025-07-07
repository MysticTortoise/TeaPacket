#pragma once
#include <d3d11.h>
#include <wrl/client.h>

namespace TeaPacket::Graphics
{
    namespace Platform
    {
        /// Platform-Specific display data for Windows.
        class WindowsModel
        {
        public:
            Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer, indexBuffer;
            size_t vertexSize, indexCount;
        };
    }
    typedef Platform::WindowsModel PlatformModel;
}

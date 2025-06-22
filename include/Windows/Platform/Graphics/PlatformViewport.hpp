#pragma once
#include "d3d11.h"

namespace TeaPacket::Graphics
{
    namespace Platform
    {
        class WindowsViewport
        {
        public:
            ID3D11RenderTargetView* renderTargetView; ///< The ID3D11RenderTargetView that draw calls to this viewport go to.
        };
    }

    typedef Platform::WindowsViewport PlatformViewport;

}
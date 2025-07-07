//
// Private Graphics functions
//

#pragma once
#include "TeaPacket/Graphics/Viewport.hpp"
#include "TeaPacket/Graphics/Color4.hpp"

namespace TeaPacket::Graphics
{
    /// Platform-specific implementation of Initialize();
    bool Pl_Initialize();
    /// Platform-specific implementation of DeInitialize();
    void Pl_DeInitialize();

    /// Platform-specific implementation of BeginRenderToDisplay();
    void Pl_BeginRenderToViewport(Viewport& display);
    /// Platform-specific implementation of FinishRenderToDisplay();
    void Pl_FinishRenderToViewport();

    void Pl_RenderViewportToDisplay(Display& display, Viewport& viewport);

    void Pl_ClearScreen(const Color4& color);


}
#pragma once

#include "TeaPacket/Graphics/Color4.hpp"

/// Functionality relating to drawing graphics to the screen.
namespace TeaPacket::Graphics
{
    class Viewport;
    class Display;

    /// Initializes any Graphics related systems.
    bool Initialize();

    /// Deinitializes all Graphics related systems
    void DeInitialize();

    /// Begins rendering to a specified Interface::Display

    /// Must always be followed by a call to FinishRenderToDisplay()
    void BeginRenderToViewport(Viewport& viewport);
    /// Concludes rendering to the current Interface::Display
    void FinishRenderToViewport();

    /// Clears the active Display with a color.
    void ClearScreen(const Color4& color);

    /// Whether all Interface functionality has been initialized.
    inline bool initialized = false;
    /// The current Interface::Display being rendered to.
    /** Will be nullptr when no Interface::Display is being rendered to. **/
    inline Viewport* currentViewport = nullptr;
}
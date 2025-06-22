//
// Includes basic, generalized engine behavior.
//
#pragma once

namespace TeaPacket
{
    /// Initialize all engine functionality.
    /// @return Whether the engine successfully initialized or not.
    bool Initialize();
    /// Returns whether the app should continue running.
    /// @return If the app should continue running.
    bool ShouldRun();
    /// Perform all engine functionality for 1 frame (or Tick).
    void Tick();
    /// De-initialize all engine functionality.
    void DeInitialize();

    /// A helper function which handles the basic engine loop automatically.
    void RunEngine();
}

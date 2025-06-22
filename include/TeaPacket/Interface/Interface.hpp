//
// Global functions for Basic I/O Functionality
//

#pragma once

/// Functionality relating to core application management, such as event-handling and process management.
namespace TeaPacket::Interface
{
    /// Initializes application information.
    void Initialize();
    /// Updates any core application info and handles events.
    void Tick();
    /// De-Initializes the application's data.
    void DeInitialize();

    /// Returns whether the application should quit or not.
    bool ShouldQuit();
    /// Queues a quit of the application. The app will return false on the next call to ShouldQuit()
    void QueueQuit();

    /// Whether all Interface functionality has been initialized.
    inline bool initialized = false;
}

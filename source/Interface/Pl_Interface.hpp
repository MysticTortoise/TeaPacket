//
// Created by Denis on 6/16/2025.
//

#pragma once

namespace TeaPacket::Interface
{
    /// Platform-specific Interface Initialization.
    bool Pl_Initialize();
    /// Platform-specific Interface DeInitialization.
    void Pl_DeInitialize();
    /// Platform-specific Interface Tick.
    void Pl_Tick();
}
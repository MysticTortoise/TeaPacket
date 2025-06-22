#pragma once

#include "windows.h"

namespace TeaPacket::Interface::Platform
{
    /// The handle to the instance of this process.
    extern HINSTANCE hInstance;
    /// The standard window class used by the engine.
    extern WNDCLASSEX wndClass;
}

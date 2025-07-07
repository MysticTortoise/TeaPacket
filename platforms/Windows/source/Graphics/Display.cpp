//
// Display implementation file for Windows using Win32
//
#include "../Interface/PlatformInterface.hpp"
#include "TeaPacket/Graphics/Display.hpp"

#include <windows.h>
#include <comdef.h>
#include <iostream>

using namespace TeaPacket::Graphics;

Display Display::Pl_RegisterDisplay(const unsigned short width, const unsigned short height, const std::string& name, bool* success)
{
    Display display = Display(width, height, name);

    RECT rect;
    SetRect(&rect, 0, 0, 1280, 720);
    AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false);
    display.platformDisplay.windowHandle = CreateWindow(
        "TeaPacket_MainWindowClass",
        name.c_str(),
        WS_OVERLAPPEDWINDOW,
        0, 0,
        rect.right - rect.left, rect.bottom - rect.top,
        0,
        0,
        Interface::Platform::instanceHandle,
        0
    );

    if (display.platformDisplay.windowHandle == NULL)
    {
        DWORD dwError = GetLastError();
        if (dwError != ERROR_CLASS_ALREADY_EXISTS)
        {
            _com_error err(HRESULT_FROM_WIN32(dwError));
            LPCTSTR errorMessage = err.ErrorMessage();
            *success = false;
            throw std::runtime_error(errorMessage);
        }
    }
    ShowWindow(display.platformDisplay.windowHandle, SW_SHOWNORMAL);

    *success = true;
    return display; 
}

void Display::Pl_DeInitialize()
{

}

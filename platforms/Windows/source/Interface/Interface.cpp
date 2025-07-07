//
// Windows-Specific Interface implementation.
//
#include "TeaPacket/Interface/Interface.hpp"
#include "Interface/Pl_Interface.hpp"
#include "TeaPacket/Graphics/Display.hpp"
#include "WindowsInterface.hpp"

#include <comdef.h>
#include <stdexcept>

using namespace TeaPacket;

LRESULT WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        Interface::QueueQuit();
        return 0;
    case WM_SIZE:
        for (Graphics::Display display : Graphics::Display::Displays)
        {
            if (display.platformDisplay.windowHandle == hWnd)
            {
                int width = LOWORD(lParam);
                int height = HIWORD(lParam);
                display.Resize(static_cast<unsigned short>(width), static_cast<unsigned short>(height));
            }
        }
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
}

bool Interface::Pl_Initialize()
{
    Platform::instanceHandle = GetModuleHandle(NULL);

    WNDCLASS windowClass;
    windowClass.style = CS_VREDRAW | CS_HREDRAW;
    windowClass.lpfnWndProc = WindowProc;
    windowClass.cbClsExtra = 0;
    windowClass.cbWndExtra = 0;
    windowClass.hInstance = Platform::instanceHandle;
    windowClass.hIcon = 0;
    windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
    windowClass.hbrBackground = 0;
    windowClass.lpszMenuName = NULL;
    windowClass.lpszClassName = "TeaPacket_MainWindowClass";

    if (!RegisterClass(&windowClass))
    {
        DWORD dwError = GetLastError();
        if (dwError != ERROR_CLASS_ALREADY_EXISTS)
        {
            _com_error err(HRESULT_FROM_WIN32(dwError));
            LPCTSTR errorMessage = err.ErrorMessage();
            throw std::runtime_error(errorMessage);
        }
    }
    return true;
}

void Interface::Pl_DeInitialize()
{

}


void Interface::Pl_Tick()
{
    MSG message;

    while(PeekMessage(&message, NULL, 0U, 0U, PM_REMOVE))
    {
        TranslateMessage(&message);
        DispatchMessage(&message);
    }
}

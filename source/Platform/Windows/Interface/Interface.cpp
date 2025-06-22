//
// Windows-Specific Interface implementation.
//
#include "TeaPacket/Interface/Interface.hpp"
#include "TeaPacket/Interface/Pl_Interface.hpp"
#include "TeaPacket/Graphics/Display.hpp"
#include "PlatformInterface.hpp"

using namespace TeaPacket;

HINSTANCE Interface::Platform::hInstance;
WNDCLASSEX Interface::Platform::wndClass;

LRESULT WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
            Interface::QueueQuit();
        return 0;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
}

void Interface::Pl_Initialize()
{
    Platform::hInstance = GetModuleHandle(NULL);

    ZeroMemory(&Platform::wndClass, sizeof(WNDCLASSEX));
    Platform::wndClass.cbSize = sizeof(WNDCLASSEX);
    Platform::wndClass.style = CS_HREDRAW | CS_VREDRAW;
    Platform::wndClass.lpfnWndProc = WindowProc;
    Platform::wndClass.hInstance = Platform::hInstance;
    Platform::wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
    Platform::wndClass.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_WINDOW);
    Platform::wndClass.lpszClassName = "MainWindowClass";

    RegisterClassEx(&Platform::wndClass);
}

void Interface::Pl_DeInitialize()
{

}


void Interface::Pl_Tick()
{
    // Handle all messages.
    MSG msg;
    for (const Graphics::Display& display : Graphics::Display::Displays)
    {
        while(PeekMessage(&msg, display.platformDisplay.windowHandle, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }
}

//
// Windows-Specific Interface implementation.
//
#include "TeaPacket/Interface/Interface.hpp"
#include "TeaPacket/Interface/Pl_Interface.hpp"
#include "TeaPacket/Graphics/Display.hpp"
#include "PlatformInterface.hpp"

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
    windowClass.hbrBackground = static_cast<HBRUSH>(GetStockObject(LTGRAY_BRUSH));
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

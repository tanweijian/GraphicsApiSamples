#include "WinUtils.h"
#include "WindowsWindow.h"
#include <cassert>

const wchar_t* sSZClassName = L"GraphicsWndClass";

static HINSTANCE sInstanceHandle = NULL;

void WinUtils::SetInstanceHandle(HINSTANCE instance)
{
    assert(!sInstanceHandle);
    sInstanceHandle = instance;
}

HINSTANCE WinUtils::GetInstanceHandle()
{
    assert(sInstanceHandle);
    return sInstanceHandle;
}

ATOM WinUtils::RegisterWindowClass()
{
    WNDCLASSEXW wcex;
    ZeroMemory(&wcex, sizeof(wcex));
    wcex.cbSize = sizeof(wcex);
    wcex.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC | CS_DBLCLKS;
    wcex.lpfnWndProc = WinUtils::PlayerWndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = WinUtils::GetInstanceHandle();
    wcex.hCursor = NULL;
    wcex.hbrBackground = NULL;
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = sSZClassName;
    return RegisterClassExW(&wcex);
}

void WinUtils::UnregisterWindowClass()
{
    UnregisterClassW(sSZClassName, WinUtils::GetInstanceHandle());
}

LRESULT WinUtils::PlayerWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    WindowsWindow* window = nullptr;
    if (message == WM_NCCREATE)
    {
        window = static_cast<WindowsWindow*>(reinterpret_cast<CREATESTRUCT*>(lParam)->lpCreateParams);
        SetWindowLongPtrW(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(window));
    }
    else
    {
        window = reinterpret_cast<WindowsWindow*>(GetWindowLongPtrA(hWnd, GWLP_USERDATA));
        return window->ProcessWndProc(hWnd, message, wParam, lParam);
    }
    return DefWindowProcW(hWnd, message, wParam, lParam);
}

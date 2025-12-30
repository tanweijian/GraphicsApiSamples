#pragma once

#include <Windows.h>

namespace WinUtils
{
    void SetInstanceHandle(HINSTANCE instance);
    HINSTANCE GetInstanceHandle();

    ATOM RegisterWindowClass();
    void UnregisterWindowClass();

    LRESULT PlayerWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
}
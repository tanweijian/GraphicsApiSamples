#pragma once

#include <Windows.h>
#include "External/SimpleMath/SimpleMath.h"

using namespace DirectX::SimpleMath;

class WindowsWindow
{
public:
    WindowsWindow();
    ~WindowsWindow();

private:
    HWND _Handle;
    Vector2 _ScreenSize;

private:
    Vector2 GetDisplaySize();

public:
    void Init();
    inline bool HasHWND() const { return _Handle != nullptr; }
    inline HWND GetHWND() const { return _Handle; }
    bool ProcessMessages();
    LRESULT ProcessWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

public:
    void Show() const;
    void SetTitle(const wchar_t* title) const;
    Vector2 GetScreenSize() const { return _ScreenSize; }
};
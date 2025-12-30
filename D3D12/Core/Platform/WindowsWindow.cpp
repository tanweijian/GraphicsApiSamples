#include "WinUtils.h"
#include "WindowsWindow.h"

const wchar_t* sWindowClassName = L"GraphicsWndClass";

WindowsWindow::WindowsWindow() : _Handle(nullptr)
{
}

WindowsWindow::~WindowsWindow()
{
    if (HasHWND())
    {
        CloseWindow(_Handle);
    }
}

Vector2 WindowsWindow::GetDisplaySize()
{
    float x = GetSystemMetrics(SM_CXSCREEN) / 2.0f;
    float y = GetSystemMetrics(SM_CYSCREEN) / 2.0f;
    if (y < 1080.0f)
    {
        y = 1080.0f;
    }
    if ((x / y) < (16.0f / 9.0f))
    {
        x = y / 9.0f * 16.0f;
    }
    return Vector2(static_cast<float>(x), static_cast<float>(y));
}

void WindowsWindow::Init()
{
    DWORD exstyle = 0;
    DWORD style = WS_OVERLAPPEDWINDOW;
    Vector2 displaySize = GetDisplaySize();
    RECT desiredRect = { 0L, 0L, static_cast<LONG>(displaySize.x), static_cast<LONG>(displaySize.y) };
    AdjustWindowRectEx(&desiredRect, style, false, exstyle);
    _ScreenSize = Vector2(static_cast<float>(desiredRect.right - desiredRect.left), static_cast<float>(desiredRect.bottom - desiredRect.top));
    _Handle = CreateWindowExW(exstyle, sWindowClassName, L"", style, CW_USEDEFAULT, CW_USEDEFAULT, static_cast<int>(_ScreenSize.x), static_cast<int>(_ScreenSize.y), NULL, NULL, WinUtils::GetInstanceHandle(), this);
}

bool WindowsWindow::ProcessMessages()
{
    MSG msg;
    msg.message = WM_NULL;
    while (PeekMessageW(&msg, NULL, 0U, 0U, PM_REMOVE))
    {
        if (WM_QUIT == msg.message)
        {
            return false;
        }

        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }

    return true;
}

LRESULT WindowsWindow::ProcessWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_DESTROY:
    {
        PostQuitMessage(0);
        break;
    }
    case WM_SIZE:
    {
        RECT rect;
        GetClientRect(hWnd, &rect);
        _ScreenSize = Vector2(static_cast<float>(rect.right - rect.left), static_cast<float>(rect.bottom - rect.top));
        break;
    }
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);

        RECT rect = { 0L, 0L, static_cast<LONG>(_ScreenSize.x), static_cast<LONG>(_ScreenSize.y) };
        GetClientRect(hWnd, &rect);
        HBRUSH hBlackBrush = CreateSolidBrush(RGB(0, 0, 0));
        FillRect(hdc, &rect, hBlackBrush);
        DeleteObject(hBlackBrush);

        EndPaint(hWnd, &ps);
        break;
    }
    }
    return DefWindowProcW(hWnd, message, wParam, lParam);
}

void WindowsWindow::Show() const
{
    if (HasHWND())
    {
        ShowWindow(_Handle, SW_SHOW);
        UpdateWindow(_Handle);
    }
}

void WindowsWindow::SetTitle(const wchar_t* title) const
{
    SetWindowTextW(_Handle, title);
}

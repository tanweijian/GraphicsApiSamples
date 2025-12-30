#include <stdio.h>
#include <Windows.h>
#include "Console.h"

static HANDLE _sHandle = nullptr;

void Console::Open()
{
    if (::AllocConsole())
    {
        // Redirect the CRT standard input, output, and error handles to the console
        FILE* pCout;
        freopen_s(&pCout, "CONIN$", "r", stdin);
        freopen_s(&pCout, "CONOUT$", "w", stdout);
        freopen_s(&pCout, "CONOUT$", "w", stderr);

        _sHandle = GetStdHandle(STD_OUTPUT_HANDLE);

        // Disable Close-Button
        HWND hwnd = GetConsoleWindow();
        if (hwnd != nullptr)
        {
            HMENU hMenu = GetSystemMenu(hwnd, FALSE);
            if (hMenu != nullptr)
            {
                DeleteMenu(hMenu, SC_CLOSE, MF_BYCOMMAND);
            }
        }
    }
}

void Console::Close()
{
    if (_sHandle != nullptr)
    {
        ::CloseHandle(_sHandle);
        _sHandle = nullptr;
    }
}
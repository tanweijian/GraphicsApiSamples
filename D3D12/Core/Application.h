#pragma once

#include "GUI.h"
#include "Core/CoreMinimal.h"
#include "Core/Misc/NonCopyable.h"
#include "Core/Platform/WinUtils.h"
#include "Core/Platform/WindowsWindow.h"
#include "Core/RHI/D3D12Options.h"
#include "Core/RHI/D3D12RHIDefinitions.h"

using Microsoft::WRL::ComPtr;

class Application : public NonCopyable
{
public:
    Application() = default;
    virtual ~Application() = default;

public:
    int Run();

private:
    void ParseCommandLine();
    void Initialize();
    void InitInternal();
    void Update();
    void Draw();
    void Exit();

protected:
    virtual void OnInit() = 0;
    virtual void OnUpdate() = 0;
    virtual void OnDraw() = 0;
    virtual void OnExit() = 0;

protected:
    WindowsWindow Window;
    D3D12Options Options;
};

#define ENTRY_MAIN(app_class)                                                                                             \
int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow) \
{                                                                                                                         \
    UNREFERENCED_PARAMETER(hPrevInstance);                                                                                \
    UNREFERENCED_PARAMETER(lpCmdLine);                                                                                    \
                                                                                                                          \
    WinUtils::SetInstanceHandle(hInstance);                                                                               \
    WinUtils::RegisterWindowClass();                                                                                      \
                                                                                                                          \
    app_class app;                                                                                                        \
    int exitCode = app.Run();                                                                                             \
                                                                                                                          \
    WinUtils::UnregisterWindowClass();                                                                                    \
                                                                                                                          \
    return exitCode;                                                                                                      \
}

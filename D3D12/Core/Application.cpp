#include "Application.h"
#include "Core/Misc/CLI11.hpp"

int Application::Run()
{
    Initialize();
    // Main loop
    bool isRunning = true;
    while (isRunning)
    {
        // Process window messages
        isRunning = Window.ProcessMessages();
        Update();
        Draw();
    }
    Exit();
    return 0;
}

void Application::InitInternal()
{
    Window.Init();
    Window.SetTitle(L"GraphicsAPI");
    Window.Show();
}

void Application::ParseCommandLine()
{
    int argc;
    LPWSTR* w_argv = CommandLineToArgvW(GetCommandLineW(), &argc);

    std::vector<std::string> argv_utf8(argc);
    std::vector<char*> argv_ptrs(argc);

    for (int i = 0; i < argc; ++i)
    {
        int wide_len = lstrlenW(w_argv[i]);
        int buf_size = WideCharToMultiByte(CP_UTF8, 0, w_argv[i], wide_len, NULL, 0, NULL, NULL);
        argv_utf8[i].resize(buf_size);
        WideCharToMultiByte(CP_UTF8, 0, w_argv[i], wide_len, &(argv_utf8[i][0]), buf_size, NULL, NULL);
        argv_ptrs[i] = &(argv_utf8[i][0]);
    }

    LocalFree(w_argv);

    CLI::App cliApp{ "GraphicsAPI App" };
    ZeroMemory(&Options, sizeof(D3D12Options));
    cliApp.add_flag("--enable-d3d12-debug", Options.EnableDebugLayer, "Enables the D3D12 validation layer");
    cliApp.add_flag("--enable-gpu-based-validation", Options.EnableGPUBasedValidation, "Enables the D3D12 GPU-based validation");
    cliApp.add_flag("--use-dred", Options.UseDRED, "Enables DRED (Device Removed Extended Data) for D3D12 device");
    cliApp.add_flag("--use-warp", Options.UseWarp, "Use WARP adapter instead of hardware GPU");
    cliApp.parse(argc, argv_ptrs.data());
}

void Application::Initialize()
{
    ParseCommandLine();
    InitInternal();
    OnInit();
}

void Application::Update()
{
    OnUpdate();
}

void Application::Draw()
{
    OnDraw();
}

void Application::Exit()
{
    OnExit();
}

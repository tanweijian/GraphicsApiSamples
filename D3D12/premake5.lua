workspace "D3D12"
    configurations { "Debug", "Release" }
    platforms { "x64" }
    architecture "x86_64"
    system "windows"

    function AddRuntimedependency(source, destination)
        postbuildcommands { ("{COPY} \"$(SolutionDir)" .. source .. "\" \"$(OutDir)" .. destination .. "/\"") }
    end

    function ProjectCommon()
        language "C++"
        cppdialect "C++20"
        cdialect "C17"
        debugdir "$(ProjectDir)"
    end

    function AddBaseModule()
        -- imgui
        includedirs "$(SolutionDir)../ThirdParty/imgui"
        files
        {
            _MAIN_SCRIPT_DIR .. "/../ThirdParty/imgui/*.h", _MAIN_SCRIPT_DIR ..  "/../ThirdParty/imgui/*.cpp", 
            _MAIN_SCRIPT_DIR ..  "/../ThirdParty/imgui/backends/imgui_impl_dx12.*", _MAIN_SCRIPT_DIR ..  "/../ThirdParty/imgui/backends/imgui_impl_win32.*"
        }

        -- SimpleMath
        files { _MAIN_SCRIPT_DIR .. "/External/SimpleMath/**.h", _MAIN_SCRIPT_DIR .. "/External/SimpleMath/**.inl", _MAIN_SCRIPT_DIR .. "/External/SimpleMath/**.cpp" }

        -- spdlog
        includedirs "$(SolutionDir)../ThirdParty/spdlog/include"
    end

    -- AgilitySDK Module
    function AddAgilitySDKModule()
        includedirs "$(SolutionDir)External/AgilitySDK/Include"
        includedirs "$(SolutionDir)External/AgilitySDK/Include/d3dx12"
        files { _MAIN_SCRIPT_DIR .. "/External/AgilitySDK/Source/**.cpp" }
        links { "d3d12", "dxgi", "dxguid" }
        AddRuntimedependency("External/AgilitySDK/Binaries/D3D12Core.dll", "D3D12")
        filter "configurations:Debug"
            AddRuntimedependency("External/AgilitySDK/Binaries/d3d12SDKLayers.dll", "D3D12")
        filter {}
    end

    -- Dxc Module
    function AddDxcModule()
        includedirs "$(SolutionDir)External/Dxc/Include"
        libdirs "$(SolutionDir)External/Dxc/StaticLibrary"
        links { "dxcompiler", "dxil"}
        AddRuntimedependency("External/Dxc/Binaries/dxcompiler.dll", "")
        AddRuntimedependency("External/Dxc/Binaries/dxil.dll", "")
    end

    -- PIX Module
    function AddPixModule()
        includedirs "$(SolutionDir)External/Pix/Include"
        libdirs "$(SolutionDir)External/Pix/StaticLibrary"
        links { "WinPixEventRuntime" }
        filter "configurations:Debug"
            AddRuntimedependency("External/Pix/Binaries/WinPixEventRuntime.dll", "")
        filter {}
    end

    for _, dir in ipairs(os.matchdirs("Samples/*")) do
        include(dir)
    end

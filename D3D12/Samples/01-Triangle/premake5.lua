project "01-Triangle"
    targetname "Triangle"
    kind "WindowedApp"
    systemversion "latest"
    
    ProjectCommon()

    includedirs { "$(SolutionDir)" }
    files { _MAIN_SCRIPT_DIR .. "/Core/**.h", _MAIN_SCRIPT_DIR .. "/Core/**.hpp", _MAIN_SCRIPT_DIR .. "/Core/**.cpp", "**.h", "**.hpp", "**.cpp"}

    AddBaseModule()
    AddAgilitySDKModule()
    AddDxcModule()

    vpaths
    {
        ["Core/*"] = { _MAIN_SCRIPT_DIR .. "/Core/**.*" },
        ["External/*"] = { _MAIN_SCRIPT_DIR .. "/External/**.*" }
    }

    filter "configurations:Debug"
        runtime "Debug"
        symbols "On"

    filter "configurations:Release"
        runtime "Release"
        optimize "Off"
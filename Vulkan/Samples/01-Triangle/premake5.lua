project "01-Triangle"
    targetname "Triangle"
    kind "WindowedApp"
    systemversion "latest"
    language "C++"
    cppdialect "C++20"

    includedirs { "$(SolutionDir)" }
    files { _MAIN_SCRIPT_DIR .. "/Core/**.h", _MAIN_SCRIPT_DIR .. "/Core/**.cpp", "**.h", "**.cpp"}

    AddVulkanModule()

    vpaths
    {
        ["Core/*"] = { _MAIN_SCRIPT_DIR .. "/Core/**.*" },
        ["External/*"] = { _MAIN_SCRIPT_DIR .. "/External/**.*" }
    }

    filter "configurations:Debug"
        runtime "Debug"
        symbols "Off"

    filter "configurations:Release"
        runtime "Release"
        optimize "On"
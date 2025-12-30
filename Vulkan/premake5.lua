workspace "Vulkan"
    configurations { "Debug", "Release" }
    platforms { "x64" }
    architecture "x86_64"
    system "windows"
    
    function AddVulkanModule()
        -- vulkan
        includedirs "$(SolutionDir)External/Vulkan/Include"
        libdirs "$(SolutionDir)External/Vulkan/StaticLibrary"
        links { "vulkan-1" }
    end

    for _, dir in ipairs(os.matchdirs("Samples/*")) do
        include(dir)
    end
workspace "Gale"
	architecture "x64"
	startproject "Gale"
	
    configurations 
    {
        "Debug",
        "Release"
    }
    
	flags
	{
		"MultiProcessorCompile"
	}
	
outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
vcpkg = "C:/dev/vcpkg/packages"

project "Gale"
    location "Gale"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"
   
    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")
    
	--pchheader "tppch.h"
	--pchsource "Tempest/src/tppch.cpp"

    files 
    { 
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp",
    }
	
	includedirs
	{
		vcpkg .. "/glfw3_x64-windows-static/include"
	}
	
	libdirs
	{
		vcpkg .. "/glfw3_x64-windows-static/lib"
	}
	
	links
	{
		"glfw3"
	}
	

    filter "configurations:Debug"
        defines { "DEBUG" }
        runtime "Debug"
        symbols "On"

    filter "configurations:Release"
        defines { "NDEBUG" }
        runtime "Release"
        optimize "On"
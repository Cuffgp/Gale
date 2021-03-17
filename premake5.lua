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

-- Include directories relative to root folder (solution directory)
IncludeDir = {}
IncludeDir["GLFW"] = "Gale/vendor/glfw/include"

LibraryDir = {}


group "Dependencies"
	include "Gale/vendor/"

project "Gale"
    location "Gale"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"
   
    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")
    
	--pchheader "glpch.h"
	--pchsource "Gale/src/tppch.cpp"

    files 
    { 
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.c",
		"%{prj.name}/src/**.hpp", 
		"%{prj.name}/src/**.cpp",
    }
	
	includedirs
	{
		"%{IncludeDir.GLFW}",
		"%{prj.name}/vendor"
	}
	
	libdirs
	{

	}
	
	links
	{
		"GLFW"
	}
	

    filter "configurations:Debug"
        defines { "DEBUG" }
        runtime "Debug"
        symbols "On"

    filter "configurations:Release"
        defines { "NDEBUG" }
        runtime "Release"
        optimize "On"
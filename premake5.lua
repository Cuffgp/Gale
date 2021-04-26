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
IncludeDir["ImGui"] = "Gale/vendor/imgui"
IncludeDir["Vulkan"] = "C:/VulkanSDK/1.2.170.0/Include"
IncludeDir["glm"] = "Gale/vendor/glm"
IncludeDir["tinyobjloader"] = "Gale/vendor/tinyobjloader"
IncludeDir["stb_image"] = "Gale/vendor/stb_image"

LibraryDir = {}
LibraryDir["Vulkan"] = "C:/VulkanSDK/1.2.170.0/Lib/vulkan-1.lib"


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
	
	pchheader "glpch.h"
	pchsource "Gale/src/glpch.cpp"

	files 
	{ 
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.c",
		"%{prj.name}/src/**.hpp", 
		"%{prj.name}/src/**.cpp",
	}
	
	includedirs
	{
		"%{prj.name}/src",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.Vulkan}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.tinyobjloader}",
		"%{IncludeDir.stb_image}",
		"%{prj.name}/vendor"
	}
	
	links
	{
		"GLFW",
		"ImGui",
		"%{LibraryDir.Vulkan}"
	}
	
	--postbuildcommands { "call %{prj.name}/assets/compile.bat" }
	

	filter "configurations:Debug"
		defines { "DEBUG" }
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		defines { "NDEBUG" }
		runtime "Release"
		optimize "On"
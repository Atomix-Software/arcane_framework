project "Arcane"
	location "Arcane"
	kind "StaticLib"
	language "C++"
	cppdialect "C++20"
	staticruntime "on"

	INCLUDE_DIR = {}
	INCLUDE_DIR["GLFW"] = "Arcane/vendors/glfw/include"
	INCLUDE_DIR["GLM"] = "Arcane/vendors/glm"
	INCLUDE_DIR["GLAD"] = "Arcane/vendors/glad/include"
	INCLUDE_DIR["STB"] = "Arcane/vendors/stb"
	INCLUDE_DIR["SPD_LOG"] = "Arcane/vendors/spdlog/include"
	INCLUDE_DIR["IMGUI"] = "Arcane/vendors/imgui"
	INCLUDE_DIR["OpenAL"] = "Arcane/vendors/openal-soft/include"

	group "Dependencies"
		include "Arcane/vendors/glfw"
		include "Arcane/vendors/glad"
		include "Arcane/vendors/imgui"
		include "Arcane/vendors/openal-soft"

	group ""
	
	buildoptions{"/utf-8"}
	
	targetdir("bin/" .. OUTPUT_DIR .. "/%{prj.name}")
	objdir("bin-int/" .. OUTPUT_DIR .. "/%{prj.name}")
	
	pchheader "arcpch.h"
	pchsource "Arcane/src/arcpch.cpp"
	
	files 
	{
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/src/**.c",
		"%{prj.name}/src/**.hpp",
		"%{prj.name}/src/**.h"
	}
	
	includedirs 
	{
		"%{prj.name}/src",
		"%{INCLUDE_DIR.GLFW}",
		"%{INCLUDE_DIR.GLM}",
		"%{INCLUDE_DIR.GLAD}",
		"%{INCLUDE_DIR.STB}",
		"%{INCLUDE_DIR.SPD_LOG}",
		"%{INCLUDE_DIR.OpenAL}",
		"%{INCLUDE_DIR.IMGUI}"
	}
	
	links 
	{
		"glfw",
		"glad",
		"ImGui",
		"openal-soft",
		"opengl32.lib"
	}

	defines 
	{
		"GLFW_INCLUDE_NONE"
	}
	
	filter "system:windows"
		systemversion "latest"
	
		defines 
		{
			"_CRT_SECURE_NO_WARNINGS"
		}
		
	filter "configurations:Debug"
		defines "ARC_DEBUG"
		runtime "Debug"
		symbols "on"
		
	filter "configurations:Release"
		defines "ARC_RELEASE"
		runtime "Release"
		optimize "on"
		
	filter "configurations:Distribution"
		defines "ARC_DIST"
		runtime "Release"
		optimize "on"
		
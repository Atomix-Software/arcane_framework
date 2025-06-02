project "Arcane"
	location "Arcane"
	kind "StaticLib"
	language "C++"
	cppdialect "C++20"
	staticruntime "on"

	INCLUDE_DIR = {}
	INCLUDE_DIR["GLFW"]    = "Arcane/vendors/glfw/include"
	INCLUDE_DIR["GLM"]     = "Arcane/vendors/glm"
	INCLUDE_DIR["GLAD"]    = "Arcane/vendors/glad/include"
	INCLUDE_DIR["STB"]     = "Arcane/vendors/stb"
	INCLUDE_DIR["SPD_LOG"] = "Arcane/vendors/spdlog/include"
	INCLUDE_DIR["IMGUI"]   = "Arcane/vendors/imgui"
	INCLUDE_DIR["OpenAL"]  = "Arcane/vendors/openal-soft/include"
	INCLUDE_DIR["SNDFILE"] = "Arcane/vendors/libsndfile/include"

	group "Dependencies"
		include "Arcane/vendors/glfw"
		include "Arcane/vendors/glad"
		include "Arcane/vendors/imgui"

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
		"%{INCLUDE_DIR.SNDFILE}",
		"%{INCLUDE_DIR.IMGUI}"
	}

	links 
	{
		"glfw",
		"glad",
		"ImGui",
		"winmm.lib",
		"opengl32.lib",
		"Arcane/vendors/openal-soft/libs/OpenAL32.lib",
		"Arcane/vendors/libsndfile/lib/sndfile.lib",
	}

	defines 
	{
		"GLFW_INCLUDE_NONE",
		"AL_LIBTYPE_STATIC"
	}
	
	filter "system:windows"
		systemversion "latest"
	
		defines 
		{
			"_CRT_SECURE_NO_WARNINGS"
		}
		
	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"
		
	filter "configurations:Release"
		runtime "Release"
		optimize "on"
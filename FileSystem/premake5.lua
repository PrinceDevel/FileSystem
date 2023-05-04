project "FileSystem"

	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "off"

	targetdir   (out_dir .. "/%{prj.name}")
	objdir      (int_dir .. "/%{prj.name}")

	pchheader "precompiled.hpp"
	pchsource "source/precompiled.cpp"

	files {
        "include/FileSystem/**.h",
		"include/FileSystem/**.hpp",
        
        "source/**.c",
		"source/**.cpp",
		"source/precompiled.hpp"
	}

	includedirs {
		"include",
		"source"
	}

	filter "configurations:Debug"
		defines "_CONFIG_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "_CONFIG_RELEASE"
		runtime "Release"
		optimize "on"
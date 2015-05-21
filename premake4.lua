solution "Thought"
	includedirs { "src" }
	includedirs { "src/include" }

	newoption {
		trigger = "no-console",
		description = "Don't show a console window with the application"
	}
	
	configurations { "Debug", "Release" }
	
	configuration { "gmake" }
		buildoptions "-std=c++11"
	
	configuration  "Debug"
		flags { "Symbols" }
		defines "_DEBUG"

	configuration "Release"
		flags { "Optimize" }
		defines "NDEBUG"

	project "thought_base"
		kind "SharedLib"
		language "C++"
		files { "src/base/**.cpp" }

	project "thought_lib"
		kind "SharedLib"
		language "C++"
		files { "src/lib/**.cpp" }
		links { "thought_base" }

	project "thoughtc_lib"
		kind "SharedLib"
		language "C++"
		files { "src/compiler/**.cpp" }
		excludes { "src/compiler/main.cpp", "**Node.cpp", "**Operator.cpp" }
		links { "thought_lib", "thought_base" }

	project "thought"
		kind "ConsoleApp"
		language "C++"
		files { "src/compiler/main.cpp" }
		links { "thought_lib", "thoughtc_lib", "thought_base" }
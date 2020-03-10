--
-- Premake 5.x build configuration script
-- Use this script to configure the project with Premake5.
--

--
-- Disable deprecation warnings for myself, so that older development
-- versions will still be able to regenerate the scripts.
--

premake.api.deprecations "off"

--
-- Define the project. Put the release configuration first so it will be the
-- default when folks build using the makefile. That way they don't have to
-- worry about the /scripts argument and all that.
--

solution "kage2d"
	configurations { "Release", "Debug" }
	platforms {"x64"}
	
group "Components"
group "Components/kage2d"
	project "kage2dutil"
		language    "C++"
		kind        "StaticLib"
		flags       { "ExtraWarnings", "NoMinimalRebuild" }
		includedirs { "$(SolutionDir)dev/sdk/include", "$(SolutionDir)dev/sdk/include/kage2dutil" }
		location "dev/components/kage2dutil"
		objdir "$(SolutionDir)temp/"
		targetdir "$(SolutionDir)dev/sdk/lib/"
		characterset ("MBCS")

		files
		{
			"dev/sdk/include/kage2dutil/**.h", 
			"dev/components/kage2dutil/**.cpp"
		}
		configuration "Debug"
			targetname  "kage2dutil_d"
			defines     "_DEBUG"
			flags       { "Symbols" }

		configuration "Release"
			targetname  "kage2dutil"
			defines     "NDEBUG"
			flags       { "OptimizeSpeed" }

		configuration "vs*"
			defines     { "_CRT_SECURE_NO_WARNINGS" }

group "Projects"
	project "base"
		language    "C++"
		kind        "ConsoleApp"
		flags       { "ExtraWarnings", "NoMinimalRebuild"}
		includedirs { "$(SolutionDir)dev/sdk/include","$(SolutionDir)dev/sdk/include/kage2dutil", "$(ProjectDir)"}
		targetdir "$(SolutionDir)bin/"
		objdir "$(SolutionDir)temp/"
		implibdir "$(SolutionDir)dev/sdk/lib/"
		libdirs {"$(SolutionDir)dev/sdk/lib/"}
		buildoptions  {"/bigobj", "/MP6"}
		location "projects/base"
		characterset ("MBCS")
		
		files
		{
			"projects/base/**.cpp",
			"projects/base/**.h"
		}
		configuration "Debug"
			targetname  "base_d"
			targetdir   "$(SolutionDir)bin"
			defines     {"_DEBUG"}
			flags       { "Symbols" }
			links {"kage2dutil_d","opengl32","winmm"}

		configuration "Release"
			targetname  "base"
			targetdir   "$(SolutionDir)bin"
			defines     {"NDEBUG", "TIXML_USE_STL"}
			flags       { "OptimizeSpeed" }
			links {"kage2dutil","opengl32","winmm"}

		configuration "vs*"
			defines     { "_CRT_SECURE_NO_WARNINGS" }

--	include "projects/exampleapp"
	
	
	
	if _ACTION == "clean" then
--		os.rmdir("bin")
--		os.rmdir("build")
	end






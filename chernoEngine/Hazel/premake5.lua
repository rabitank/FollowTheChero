

include "./vendor/premake/premake_customization/solution_items.lua"
include "Dependencies.lua"



-- premake5.lua
workspace "Hazel"
   architecture "x64"
   configurations { "Debug", "Release","Dist" }
   startproject "Hazelnut"
   cppdialect "C++17"


   solution_items
   {
		".editorconfig"
   }

   flags
   {
   	"MultiProcessorCompile"
   }

 outputdir = "%{cfg.buildcfg}-%{cfg.sys}-%{cfg.architecture}"

group "Dependencies"
   include "vendor/premake" 
   include "Hazel/vendor/imgui" 
   include "Hazel/vendor/GLFW" 
   include "Hazel/vendor/yaml-cpp" 
   include "Hazel/vendor/Glad" --incldue(copy) the premake.lua in Glad to here  = add project GLAW
   include "Hazel/vendor/Box2D"
group ""

group "Core"
	include "Hazel"
group ""

group "Tools"
	include "Hazelnut"
group ""

group "Misc"
	include "Sandbox"
group ""


 
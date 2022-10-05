-- premake5.lua
workspace "Hazel"
   architecture "x64"
   configurations { "Debug", "Release","Dist" }
   startproject "Sandbox"

   

 outputdir = "%{cfg.buildcfg}-%{cfg.sys}-%{cfg.architecture}"

 IncludeDir = {}
 IncludeDir["GLFW"] = "Hazel/vendor/GLFW/include"
 IncludeDir["Glad"] = "Hazel/vendor/Glad/include"
 IncludeDir["ImGui"] = "Hazel/vendor/imgui" -- yes, imgui.h is out 
 IncludeDir["glm"] = "Hazel/vendor/glm" 

project "Hazel"
   location "Hazel" --���·��
   kind "SharedLib"  -- =dll
   language "C++"
   staticruntime "off" --运行时库设为 dll

   targetdir ("bin/"..outputdir.. "/%{prj.name}")
   objdir ("bin-int/"..outputdir.. "/%{prj.name}")



   files
   {
        "%{prj.name}/src/**.cpp",
        "%{prj.name}/src/**.h",
        "%{prj.name}/vendor/glm/glm/**.hpp",
        "%{prj.name}/vendor/glm/glm/**.inl"

   }

   
   pchheader "hzpch.h"
   pchsource "Hazel/src/hzpch.cpp" --vs �������ع�

   includedirs
   {
        "%{prj.name}/src",
        "%{prj.name}/vendor/spdlog/include",
        "%{IncludeDir.GLFW}",
        "%{IncludeDir.Glad}",
        "%{IncludeDir.ImGui}",
        "%{IncludeDir.glm}"
   }

   links
   {
        "GLFW", --reference
        "Glad",
        "ImGui",
        "opengl32.lib"

   }

   filter "system:windows"
      cppdialect "C++17"
      systemversion "latest"

      defines
      {
         "HZ_BUILD_DLL",
         "HZ_PLATFORM_WINDOWS",
         "_WINDLL",
         "GLFW_INCLUDE_NONE"-- delete gl in glfw

      }
      postbuildcommands 
      {
        ("{COPY} %{cfg.buildtarget.relpath} \"../bin/"..outputdir.."/Sandbox\"") --copy from to .todir is baed on fromdir
        --fixed the problem : before we copy we don't have the dir if we just first build the solution
      }


   
   filter "configurations:Debug"
        defines "HZ_DEBUG"
        runtime "Debug" --设置运行时库为 对应 debug 版本 MDd
        symbols "On"

   filter "configurations:Release"
        defines "HZ_RELEASE"
        runtime "Release"
        optimize "On" --���������Ż�

   filter "configurations:Dist"
      defines "HZ_DIST"
      runtime "Release"
      optimize "On" --���������Ż�

project "Sandbox"
   location "Sandbox" --���·��
   kind "ConsoleApp"  -- =dll
   language "C++"
   staticruntime "off" --运行时库设为 dll

   targetdir ("bin/"..outputdir.."/%{prj.name}")
   objdir ("bin-int/"..outputdir.."/%{prj.name}")

   files
   {
        "%{prj.name}/src/**.cpp",
        "%{prj.name}/src/**.h"
   }
   includedirs
   {
        
        "Hazel/src",
        "Hazel/vendor/spdlog/include",
        "%{IncludeDir.glm}"

   }

   links
   {
        "Hazel"
   }

   filter "system:windows"
      cppdialect "C++17"
      systemversion "latest"

      defines
      {
         "HZ_PLATFORM_WINDOWS"
      }

   
   filter "configurations:Debug"
        defines "HZ_DEBUG"
        runtime "Debug"
        symbols "On"

   filter "configurations:Release"  
        defines "HZ_RELEASE"
        runtime "Release"
        optimize "On" --���������Ż�

   filter "configurations:Dist"
      defines "HZ_DIST"
      runtime "Release"
      optimize "On" --��
 
group "Dependencies"
    include "Hazel/vendor/imgui" 
    include "Hazel/vendor/GLFW" 
    include "Hazel/vendor/Glad" --incldue(copy) the premake.lua in Glad to here  = add project GLAW
  

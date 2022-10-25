
-- premake5.lua
workspace "Hazel"
   architecture "x64"
   configurations { "Debug", "Release","Dist" }
   startproject "Sandbox"
   cppdialect "C++17"

   

 outputdir = "%{cfg.buildcfg}-%{cfg.sys}-%{cfg.architecture}"

 IncludeDir = {}
 IncludeDir["GLFW"] = "Hazel/vendor/GLFW/include"
 IncludeDir["Glad"] = "Hazel/vendor/Glad/include"
 IncludeDir["ImGui"] = "Hazel/vendor/imgui" -- yes, imgui.h is out 
 IncludeDir["glm"] = "Hazel/vendor/glm" 
 IncludeDir["stb_image"] = "Hazel/vendor/stb_image" 
 
 project "Hazel"
 location "Hazel" --���·��
 kind "StaticLib"  -- now lib     =dll
 language "C++"
 staticruntime "on" --运行时库设为静态链接(运行时库被包含在lib中) for lib  // dll 实际上是动态链接所在环境的运行时库,这会导致在不同电脑环境中用不同版本的运行时库
 
 targetdir ("bin/"..outputdir.. "/%{prj.name}")
 objdir ("bin-int/"..outputdir.. "/%{prj.name}")
 
 
 
 files
 {
      "%{prj.name}/src/**.cpp",
      "%{prj.name}/src/**.h",
        "%{prj.name}/vendor/glm/glm/**.hpp",
        "%{prj.name}/vendor/glm/glm/**.inl",
        "%{prj.name}/vendor/stb_image/**.cpp",
        "%{prj.name}/vendor/stb_image/**.h"
 }

 defines
 {
      "_CRT_SECURE_NO_WARNINGS"
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
          "%{IncludeDir.glm}",
          "%{IncludeDir.stb_image}"
     }
     
     links
     {
          "GLFW", --reference
          "Glad",
          "ImGui",
          "opengl32.lib"
          
     }
     
     filter "system:windows"
     systemversion "latest"
     
     defines
     {
          "HZ_BUILD_DLL",
          "HZ_PLATFORM_WINDOWS",
          "_WINDLL",
          "GLFW_INCLUDE_NONE"-- delete gl in glfw
          
     }
      --we dont need copy 
      
      
      
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
      cppdialect "C++17"
      staticruntime "on" --运行时库设为 静态链接
      
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
               "Hazel/vendor",
               "%{IncludeDir.glm}"
               
          }
          
          links
          {
               "Hazel"
          }
          
          filter "system:windows"
          systemversion "latest"
          
          defines
          {
               "HZ_PLATFORM_WINDOWS"
          }
          
          
   filter "configurations:Debug"
   defines "HZ_DEBUG"
   runtime "Debug"
   symbols "On" --debug symbols on
   
   filter "configurations:Release"  
   defines "HZ_RELEASE"
   runtime "Release"
   optimize "On" --开启优化
   
   filter "configurations:Dist"
   defines "HZ_DIST"
   runtime "Release"
   optimize "On" --��
   
   group "Dependencies"
   include "Hazel/vendor/imgui" 
   include "Hazel/vendor/GLFW" 
   include "Hazel/vendor/Glad" --incldue(copy) the premake.lua in Glad to here  = add project GLAW
   
   
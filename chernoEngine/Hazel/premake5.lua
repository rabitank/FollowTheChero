-- premake5.lua
workspace "Hazel"
   architecture "x64"
   configurations { "Debug", "Release","Dist" }

 outputdir = "%{cfg.buildcfg}-%{cfg.sys}-%{cfg.architecture}"


project "Hazel"
   location "Hazel" --相对路径
   kind "SharedLib"  -- =dll
   language "C++"

   targetdir ("bin/"..outputdir.. "/%{prj.name}")
   objdir ("bin-int/"..outputdir.. "/%{prj.name}")

   files
   {
        "%{prj.name}/src/**.cpp",
        "%{prj.name}/src/**.h"

   }

   includedirs
   {
        "%{prj.name}/src",
        "%{prj.name}/vendor/spdlog/include"
   }

   filter "system:windows"
      cppdialect "C++17"
      staticruntime "On" --静态链接运行时库  运行时库是c++提供的标准库的基础函数库dll("/MD")一般是动态链接,但相同代码编译出的静态库lib也被叫运行时库("/MT")
      systemversion "10.0"

      defines
      {
         "HZ_BUILD_DLL",
         "HZ_PLATFORM_WINDOWS",
         "_WINDLL"

      }
      postbuildcommands 
      {
        ("{COPY} %{cfg.buildtarget.relpath} ../bin/"..outputdir.."/Sandbox") --copy from to .todir is baed on fromdir
      }
   
   filter "configurations:Debug"
        defines "HZ_DEBUG"
        symbols "On"

   filter "configurations:Release"
        defines "HZ_RELEASE"
        optimize "On" --开启编译优化

   filter "configurations:Dist"
      defines "HZ_DIST"
      optimize "On" --开启编译优化

project "Sandbox"
   location "Sandbox" --相对路径
   kind "ConsoleApp"  -- =dll
   language "C++"

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
        "Hazel/vendor/spdlog/include"

   }

   links
   {
        "Hazel"
   }

   filter "system:windows"
      cppdialect "C++17"
      staticruntime "On" --静态链接运行时库  运行时库是c++提供的标准库的基础函数库dll("/MD")一般是动态链接,但相同代码编译出的静态库lib也被叫运行时库("/MT")
      systemversion "10.0"

      defines
      {
         "HZ_PLATFORM_WINDOWS"
      }

   
   filter "configurations:Debug"
        defines "HZ_DEBUG"
        symbols "On"

   filter "configurations:Release"
        defines "HZ_RELEASE"
        optimize "On" --开启编译优化

   filter "configurations:Dist"
      defines "HZ_DIST"
      optimize "On" --开
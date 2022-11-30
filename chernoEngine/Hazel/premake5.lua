
-- premake5.lua
workspace "Hazel"
   architecture "x64"
   configurations { "Debug", "Release","Dist" }
   startproject "Hazelnut"
   cppdialect "C++17"

   

 outputdir = "%{cfg.buildcfg}-%{cfg.sys}-%{cfg.architecture}"


 VULKAN_SDK = os.getenv("VULKAN_SDK")

 IncludeDir = {}
 IncludeDir["GLFW"] = "Hazel/vendor/GLFW/include"
 IncludeDir["Glad"] = "Hazel/vendor/Glad/include"
 IncludeDir["ImGui"] = "Hazel/vendor/imgui" -- yes, imgui.h is out 
 IncludeDir["glm"] = "Hazel/vendor/glm" 
 IncludeDir["stb_image"] = "Hazel/vendor/stb_image" 
 IncludeDir["entt"] = "Hazel/vendor/entt/include" 
 IncludeDir["yaml_cpp"] = "Hazel/vendor/yaml-cpp/include" 
 IncludeDir["ImGuizmo"] = "Hazel/vendor/ImGuizmo" 

 IncludeDir["shaderc"] = "Hazel/vendor/shaderc/include"
 IncludeDir["SPIRV_Cross"] = "Hazel/vendor/SPIRV-Cross"
 IncludeDir["VulkanSDK"] = "%{VULKAN_SDK}/Include"

 
 
 BinDir={}
 BinDir["VulkanSDK_Bin"] = "%{VULKAN_SDK}/Bin"

 Dlls={}
 Dlls["ShaderC_Debug"]="%{BinDir.VulkanSDK_Bin}/shaderc_sharedd.dll"
 Dlls["ShaderC_release"]="%{BinDir.VulkanSDK_Bin}/shaderc_shared.dll"

 LibraryDir = {}
 
 LibraryDir["VulkanSDK"] = "%{VULKAN_SDK}/Lib"
 LibraryDir["VulkanSDK_Debug"] = "%{wks.location}/Hazel/vendor/VulkanSDK/Lib"
 
 Library = {}
 Library["Vulkan"] = "%{LibraryDir.VulkanSDK}/vulkan-1.lib"
 Library["VulkanUtils"] = "%{LibraryDir.VulkanSDK}/VkLayer_utils.lib"
 
 Library["ShaderC_Debug"] = "%{LibraryDir.VulkanSDK_Debug}/shaderc_sharedd.lib"
 Library["SPIRV_Cross_Debug"] = "%{LibraryDir.VulkanSDK_Debug}/spirv-cross-cored.lib"
 Library["SPIRV_Cross_GLSL_Debug"] = "%{LibraryDir.VulkanSDK_Debug}/spirv-cross-glsld.lib"
 Library["SPIRV_Tools_Debug"] = "%{LibraryDir.VulkanSDK_Debug}/SPIRV-Toolsd.lib"
 
 Library["ShaderC_Release"] = "%{LibraryDir.VulkanSDK}/shaderc_shared.lib"
 Library["SPIRV_Cross_Release"] = "%{LibraryDir.VulkanSDK}/spirv-cross-core.lib"
 Library["SPIRV_Cross_GLSL_Release"] = "%{LibraryDir.VulkanSDK}/spirv-cross-glsl.lib"


 project "Hazel"
 location "Hazel" --���·��
 kind "StaticLib"  -- now lib     =dll
 language "C++"
 staticruntime "off" -- 运行时库设为动态Md*  //运行时库设为静态链接(运行时库被包含在lib中) for lib  // dll 实际上是动态链接所在环境的运行时库,这会导致在不同电脑环境中用不同版本的运行时库
 
 targetdir ("bin/"..outputdir.. "/%{prj.name}")
 objdir ("bin-int/"..outputdir.. "/%{prj.name}")
 
 
 
 
 files
 {
      "%{prj.name}/src/**.cpp",
      "%{prj.name}/src/**.h",
      "%{prj.name}/vendor/glm/glm/**.hpp",
      "%{prj.name}/vendor/glm/glm/**.inl",
      "%{prj.name}/vendor/stb_image/**.cpp",
      "%{prj.name}/vendor/stb_image/**.h",

      "%{prj.name}/vendor/ImGuizmo/ImGuizmo.h",
      "%{prj.name}/vendor/ImGuizmo/ImGuizmo.cpp" 
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
          "%{IncludeDir.stb_image}",
          "%{IncludeDir.entt}",
          "%{IncludeDir.yaml_cpp}",
          "%{IncludeDir.ImGuizmo}",

          "%{IncludeDir.VulkanSDK}"
     }
     
     links
     {
          "GLFW", --reference
          "Glad",
          "ImGui",
          "opengl32.lib",
          "yaml-cpp"

          
     }

     filter "files:Hazel/vendor/ImGuizmo/**.cpp"
     flags { "NoPCH" } --for cpp in ImGUizmo don't use pchheader(预编译头)
     
     filter "system:windows"
     systemversion "latest"
     
     defines
     {
          --"_WINDLL",
          "_CRT_SECURE_NO_WARNINGS",
          "GLFW_INCLUDE_NONE"-- delete gl in glfw
          
     }
      --we dont need copy 
      
      
      
      filter "configurations:Debug"
      defines "HZ_DEBUG"
      runtime "Debug" --设置运行时库为 对应 debug 版本 MDd
      symbols "On"
        links
        {
          "%{Library.ShaderC_Debug} ",
          "%{Library.SPIRV_Cross_Debug}",
          "%{Library.SPIRV_Cross_GLSL_Debug}"
     
        }

        postbuildcommands -- build后的自定义命令
        {
            ("{COPY} %{Dlls.ShaderC_Debug} ../bin/" .. outputdir .. "/Hazelnut") --拷贝引擎dll库到sanbox.exe的同一目录下去
        }

      filter "configurations:Release"
      defines "HZ_RELEASE"
      runtime "Release"
      optimize "On" --���������Ż�
        links
        {
          "%{Library.ShaderC_Release} ",
          "%{Library.SPIRV_Cross_Release}",
          "%{Library.SPIRV_Cross_GLSL_Release}"
        }

        postbuildcommands -- build后的自定义命令
        {
            ("{COPY} %{Dlls.ShaderC_Release} ../bin/" .. outputdir .. "/Hazelnut") --拷贝引擎dll库到sanbox.exe的同一目录下去
        }

      filter "configurations:Dist"
      defines "HZ_DIST"
      runtime "Release"
      optimize "On" --���������Ż�
        links
        {
          "%{Library.ShaderC_Release} ",
          "%{Library.SPIRV_Cross_Release}",
          "%{Library.SPIRV_Cross_GLSL_Release}"
        }

        postbuildcommands -- build后的自定义命令
        {
            ("{COPY} %{Dlls.ShaderC_Release} ../bin/" .. outputdir .. "/Hazelnut") --拷贝引擎dll库到sanbox.exe的同一目录下去
        }
   
        

project "Sandbox"
      location "Sandbox" --���·��
      kind "ConsoleApp"  -- =dll
      language "C++"
      cppdialect "C++17"
      staticruntime "off" --运行时库设为 dongtai链接
      
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
           "%{IncludeDir.glm}",
           "%{IncludeDir.entt}"
           
      }
      
      links
      {
           "Hazel"
      }
      
      filter "system:windows"
      systemversion "latest"
      
      defines
      {
           --"HZ_PLATFORM_WINDOWS"
      }
          
          
   filter "configurations:Debug"
   defines "HZ_DEBUG"
   runtime "Debug"
   symbols "off" --debug symbols on
   
   filter "configurations:Release"  
   defines "HZ_RELEASE"
   runtime "Release"
   optimize "off" --开启优化
   
   filter "configurations:Dist"
   defines "HZ_DIST"
   runtime "Release"
   optimize "off" --��
   


   project "Hazelnut"
      location "Hazelnut" --���·��
      kind "ConsoleApp"  -- =dll
      language "C++"
      cppdialect "C++17"
      staticruntime "off" --运行时库设为 动态链接
      
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
           "%{IncludeDir.glm}",
           "%{IncludeDir.entt}",
           "%{IncludeDir.ImGuizmo}"
           
      }
      
      links
      {
           "Hazel"
      }
      
      filter "system:windows"
      systemversion "latest"
      
      defines
      {
           --"HZ_PLATFORM_WINDOWS"
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
   include "Hazel/vendor/yaml-cpp" 
   include "Hazel/vendor/Glad" --incldue(copy) the premake.lua in Glad to here  = add project GLAW
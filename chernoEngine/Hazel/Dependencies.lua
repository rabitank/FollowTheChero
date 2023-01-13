
-- premake5.lua
workspace "Hazel"
   architecture "x64"
   configurations { "Debug", "Release","Dist" }
   startproject "Hazelnut"
   cppdialect "C++17"

   

 outputdir = "%{cfg.buildcfg}-%{cfg.sys}-%{cfg.architecture}"


 VULKAN_SDK = os.getenv("VULKAN_SDK")

 IncludeDir = {}
 IncludeDir["GLFW"] = "%{wks.location}/Hazel/vendor/GLFW/include"
 IncludeDir["Glad"] = "%{wks.location}/Hazel/vendor/Glad/include"
 IncludeDir["ImGui"] = "%{wks.location}/Hazel/vendor/imgui" -- yes, imgui.h is out 
 IncludeDir["glm"] = "%{wks.location}/Hazel/vendor/glm" 
 IncludeDir["Box2D"] = "%{wks.location}/Hazel/vendor/Box2D/include" 
 IncludeDir["stb_image"] = "%{wks.location}/Hazel/vendor/stb_image" 
 IncludeDir["entt"] = "%{wks.location}/Hazel/vendor/entt/include" 
 IncludeDir["yaml_cpp"] = "%{wks.location}/Hazel/vendor/yaml-cpp/include" 
 IncludeDir["ImGuizmo"] = "%{wks.location}/Hazel/vendor/ImGuizmo" 

 IncludeDir["shaderc"] = "%{wks.location}/Hazel/vendor/shaderc/include"
 IncludeDir["SPIRV_Cross"] = "%{wks.location}/Hazel/vendor/SPIRV-Cross"
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

 
 -- Windows
 Library["WinSock"] = "Ws2_32.lib"
 Library["WinMM"] = "Winmm.lib"
 Library["WinVersion"] = "Version.lib"
 Library["BCrypt"] = "Bcrypt.lib"

 
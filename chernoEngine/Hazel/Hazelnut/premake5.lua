  project "Hazelnut"
      --location "Hazelnut" --���·��
      kind "ConsoleApp"  -- =dll
      language "C++"
      cppdialect "C++17"
      staticruntime "off" --运行时库设为 动态链接
      
	  targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	  objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")
      
	  files
	  {
	       "src/**.h",
	       "src/**.cpp"
	  }

      includedirs
      {
           
           "%{wks.location}/Hazel/vendor/spdlog/include",
           "%{wks.location}/Hazel/src",
           "%{wks.location}/Hazel/vendor",
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
project "Sandbox"
      location "Sandbox" --���·��
      kind "ConsoleApp"  -- =dll
      language "C++"
      cppdialect "C++17"
      staticruntime "off" --运行时库设为 dongtai链接
      
	  targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	  objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")
      
	  files
	  {
	   "src/**.h",
	   "src/**.cpp"
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
   
project "Hazel"
 location "Hazel" 
 kind "StaticLib"  -- now lib     =dll
 language "C++"
 staticruntime "off" -- 运行时库设为动态Md*  //运行时库设为静态链接(运行时库被包含在lib中) for lib  // dll 实际上是动态链接所在环境的运行时库,这会导致在不同电脑环境中用不同版本的运行时库
 
 targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
 objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")
 
     files
     {
          "src/**.cpp",
          "src/**.h",
          "vendor/glm/glm/**.hpp",
          "vendor/glm/glm/**.inl",
          "vendor/stb_image/**.cpp",
          "vendor/stb_image/**.h",

          "vendor/ImGuizmo/ImGuizmo.h",
          "vendor/ImGuizmo/ImGuizmo.cpp" 
     }

     defines
     {
          "_CRT_SECURE_NO_WARNINGS",
          "GLFW_INCLUDE_NONE"
     }
     

  
     pchheader "hzpch.h"
     pchsource "src/hzpch.cpp" --vs �������ع�
     
     includedirs
     {
          "src",
          "vendor/spdlog/include",
          "%{IncludeDir.Box2D}",
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
          "Box2D",
          "GLFW", --reference
          "Glad",
          "ImGui",
          "opengl32.lib",
          "yaml-cpp"

          
     }

     filter "files:vendor/ImGuizmo/**.cpp"
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
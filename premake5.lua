workspace "VolumetricLights"
   configurations { "Release", "Debug" }
   architecture "x86"
   location "build"
   cppdialect "C++latest"
   kind "SharedLib"
   language "C++"
   targetdir "bin/%{cfg.buildcfg}"
   targetextension ".asi"
   buildoptions { "/dxifcInlineFunctions- /Zc:__cplusplus /utf-8" }
   staticruntime "On"
   characterset ("Unicode")
   multiprocessorcompile ("On")
   startproject "VolumetricLights"

   defines { "_CRT_SECURE_NO_WARNINGS" }

   includedirs { "source" }
   includedirs { "dependencies" }
   includedirs { "includes" }
   files { "source/**.h", "source/*.hpp", "source/*.cpp", "source/*.hxx", "source/**.ixx" }

   includedirs { "dependencies/injector/safetyhook/include" }
   includedirs { "dependencies/injector/zydis" }
   includedirs { "dependencies/Hooking.Patterns" }
   includedirs { "dependencies/injector/include" }
   includedirs { "dependencies/IniReader" }
   files { "dependencies/Hooking.Patterns/Hooking.Patterns.h", "dependencies/Hooking.Patterns/Hooking.Patterns.cpp" }
   files { "dependencies/injector/safetyhook/include/**.hpp", "dependencies/injector/safetyhook/src/**.cpp" }
   files { "dependencies/injector/zydis/**.h", "dependencies/injector/zydis/**.c" }
   files { "resources/*.ini" }

   filter "configurations:Debug"
      defines { "DEBUG" }
      symbols "On"

   filter "configurations:Release"
      defines { "NDEBUG" }
      optimize "On"

project "VolumetricLights"
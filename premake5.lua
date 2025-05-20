workspace "engine"
   architecture "x86_64"
   configurations { "Debug", "Release"}

include 'external/premake5.lua'

project "engine"
   kind "SharedLib"
   language "C++"
   cppdialect "c++17"
	warnings "Extra"
   targetdir "bin"
   includedirs { "engine", "external", "external/glew/include", "external/glfw/include" }
   files { "engine/**.cpp", "engine/**.hpp" }
   
   links { "GL", "glfw", "glew" }

   filter "configurations:Debug"
      defines { "DEBUG" }
      symbols "On"

   filter "configurations:Release"
      defines { "NDEBUG" }
      optimize "On"

project "editor"
   kind "ConsoleApp"
   language "C++"
   cppdialect "c++17"
	warnings "Extra"
   targetdir "bin"
   includedirs { "editor", "engine", "external" }
   files { "editor/**.cpp", "editor/**.hpp" }

   links { "engine", "imgui", "imguizmo" }

   filter "configurations:Debug"
      defines { "DEBUG" }
      symbols "On"

   filter "configurations:Release"
      defines { "NDEBUG" }
      optimize "On"

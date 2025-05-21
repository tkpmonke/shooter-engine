workspace "engine"
   configurations { "Debug", "Release"}
	architecture "x86_64"

include 'external/premake5.lua'

project "engine"
   kind "SharedLib"
   language "C++"
   cppdialect "c++17"
	warnings "Extra"
   targetdir "bin"
   includedirs { "engine", "external", "external/glew/include", "external/glfw/include", "external/rgfw" }
   files { "engine/**.cpp", "engine/**.hpp" }
   
   links { "glew", "rgfw" }

   filter "system:Unix"
	   links { "GL", "Xrandr", "X11" }
    filter "system:Windows"
        links { "opengl32" }

   filter "configurations:Debug"
      defines { "DEBUG" }
      symbols "On"

   filter "configurations:Release"
        defines { "NDEBUG" }
        optimize "On"
        symbols "Off"


project "editor"
   kind "ConsoleApp"
   language "C++"
   cppdialect "c++17"
	warnings "Extra"
   targetdir "bin"
   includedirs { "editor", "engine", "external", "external/glew/include", "external/rgfw" }
   files { "editor/**.cpp", "editor/**.hpp" }

   links { "engine", "imguizmo", "imgui", "rgfw" }

   filter "configurations:Debug"
      defines { "DEBUG" }
      symbols "On"

   filter "configurations:Release"
      defines { "NDEBUG" }
      optimize "On"
	    symbols "Off"

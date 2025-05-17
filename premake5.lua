workspace "engine"
   architecture "x86_64"
   configurations { "Debug", "Release"}

project "engine"
   kind "SharedLib"
   language "C++"
   cppdialect "c++17"
	warnings "Extra"
   targetdir "bin"
   includedirs { "engine" }
   files { "engine/**.cpp", "engine/**.hpp" }
   
   links { "GL", "glfw", "GLEW" }

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
   includedirs { "editor", "engine" }
   files { "editor/**.cpp", "editor/**.hpp" }

   links { "engine" }

   filter "configurations:Debug"
      defines { "DEBUG" }
      symbols "On"

   filter "configurations:Release"
      defines { "NDEBUG" }
      optimize "On"

project "imgui"
   kind "SharedLib"
   language "C++"
   cppdialect "c++17"
	warnings "Extra"
   targetdir "../bin"
	objdir "../obj"
   includedirs { "imgui" }
   files { "imgui/**.cpp", "imgui/**.h" }
   
   links { "glfw", "GL" }

   filter "configurations:Debug"
      defines { "DEBUG" }
      symbols "On"

   filter "configurations:Release"
      defines { "NDEBUG" }
      optimize "On"

project "imguizmo"
   kind "SharedLib"
   language "C++"
   cppdialect "c++17"
	warnings "Extra"
   targetdir "../bin"
	objdir "../obj"
   includedirs { "imgui" }
   files { "imguizmo/**.cpp", "imguizmo/**.h" }
   
   links { "imgui" }

   filter "configurations:Debug"
      defines { "DEBUG" }
      symbols "On"

   filter "configurations:Release"
      defines { "NDEBUG" }
      optimize "On"

project "glew"
   kind "SharedLib"
   language "C"
   cdialect "c99"
   targetdir "../bin"
	objdir "../obj"
   includedirs { "glew/include" }
   files { "glew/**.c", "glew/**.h" }
   
   links { "GL", "m" }

   defines { "GLEW_NO_GLU", "GLEW_SHARED" }

   filter "configurations:debug"
      defines { "DEBUG" }
      --optimize "On"
      symbols "On"

   filter "configurations:release"
      defines { "NDEBUG" }
      optimize "On"

project "glfw"
   kind "SharedLib"
   language "C++"
   cppdialect "c++17"
   targetdir "../bin"
	objdir "../obj"
   includedirs { "glfw/include" }

   files { "glfw/**.c", "glfw/**.h" }

   filter { "system:linux" }
       defines { "_GLFW_X11" }
       links { "X11", "Xrandr", "Xinerama", "Xi", "m", "dl", "pthread" }

   filter "system:windows"
       defines { "_GLFW_WIN32" }
       links { "gdi32", "user32", "shell32" }
   filter "configurations:debug"
      defines { "DEBUG" }
      --optimize "On"
      symbols "On"

   filter "configurations:release"
      defines { "NDEBUG" }
      optimize "On"

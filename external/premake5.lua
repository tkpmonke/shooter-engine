project "imgui"
	kind "StaticLib"
	language "C++"
	cppdialect "c++17"
	warnings "Extra"
	targetdir "../bin"
	objdir "../obj"
	includedirs { "imgui" }
	files { "imgui/**.cpp", "imgui/**.h" }
   
	filter "configurations:Debug"
	  defines { "DEBUG" }
	  symbols "On"

	filter "configurations:Release"
		defines { "NDEBUG" }
		optimize "On"

project "imguizmo"
   kind "StaticLib"
   language "C++"
   cppdialect "c++17"
	warnings "Extra"
   targetdir "../bin"
	objdir "../obj"
   includedirs { "imgui" }
   files { "imguizmo/**.cpp", "imguizmo/**.h" }
   
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
   
	filter "platforms:Linux"
		links { "GL", "m" }
	filter "platforms:Windows"
		links { "opengl32" }

   defines { "GLEW_NO_GLU", "GLEW_SHARED" }

   filter "configurations:debug"
	  defines { "DEBUG" }
	  --optimize "On"
	  symbols "On"

   filter "configurations:release"
	  defines { "NDEBUG" }
	  optimize "On"

project "rgfw"
   kind "SharedLib"
   language "C"
   cdialect "c99"
   targetdir "../bin"
	objdir "../obj"
   includedirs { "rgfw" }
   files { "rgfw/**.c", "rgfw/**.h" }

	--filter "platforms:Linux"
		links { "Xrandr", "X11", "Xinerama", "Xi", "m", "dl", "pthread", "GL" }

   filter "configurations:debug"
	  defines { "DEBUG" }
	  --optimize "On"
	  symbols "On"

   filter "configurations:release"
	  defines { "NDEBUG" }
	  optimize "On"

--	we now use rgfw, here just in case
--project "glfw"
--   kind "StaticLib"
--   language "C++"
--   cppdialect "c++17"
--   targetdir "../bin"
--	objdir "../obj"
--   includedirs { "glfw/include" }
--
--   files { "glfw/**.c", "glfw/**.h" }
--
--   filter { "system:linux" }
--	   defines { "_GLFW_X11" }
--	   links { "X11", "Xrandr", "Xinerama", "Xi", "m", "dl", "pthread" }
--
--   filter "system:windows"
--	   defines { "_GLFW_WIN32" }
--	   links { "gdi32", "user32", "shell32" }
--   filter "configurations:debug"
--	  defines { "DEBUG" }
--	  --optimize "On"
--	  symbols "On"
--
--   filter "configurations:release"
--	  defines { "NDEBUG" }
--	  optimize "On"

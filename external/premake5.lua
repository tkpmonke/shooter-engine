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
		symbols "Off"
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
	  symbols "Off"

project "glew"
   kind "SharedLib"
   language "C"
   cdialect "c99"
   targetdir "../bin"
	objdir "../obj"
   includedirs { "glew/include" }
   files { "glew/**.c", "glew/**.h" }
 
	filter "system:Linux"
		links { "GL", "m" }
	filter "system:Windows"
		links { "opengl32" }

   defines { "GLEW_NO_GLU", "GLEW_SHARED" }

   filter "configurations:Debug"
	  defines { "DEBUG" }
	  --optimize "On"
	  symbols "On"

   filter "configurations:Release"
	  defines { "NDEBUG" }
	  symbols "Off"
	  optimize "On"

project "rgfw"
   kind "SharedLib"
   language "C"
   cdialect "c99"
   targetdir "../bin"
	objdir "../obj"
   includedirs { "rgfw" }
   files { "rgfw/**.c", "rgfw/**.h" }

	filter "system:Linux"
		links { "Xrandr", "X11", "Xinerama", "Xi", "m", "dl", "pthread", "GL" }

   filter "configurations:Debug"
	  defines { "DEBUG" }
	  --optimize "On"
	  symbols "On"

   filter "configurations:Release"
	  defines { "NDEBUG" }
	  symbols "Off"
	  optimize "On"

project "yaml-cpp"
   kind "SharedLib"
   language "C++"
   cppdialect "c++17"
   targetdir "../bin"
	objdir "../obj"
   includedirs { "yaml-cpp" }
   files { "yaml-cpp/**.cpp" }

   filter "configurations:Debug"
	  defines { "DEBUG" }
	  --optimize "On"
	  symbols "On"

   filter "configurations:Release"
	  defines { "NDEBUG" }
	  symbols "Off"
	  optimize "On"

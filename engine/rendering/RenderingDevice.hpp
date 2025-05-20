#pragma once

#include "Mesh.hpp"
#include "Shader.hpp"

namespace engine::rendering {
	typedef void RenderObject;
	class RenderingDevice {
	public:
		enum graphics_api {
			graphics_api_opengl = 0,
			graphics_api_vulkan = 1,
		};

		static void create_rendering_device(graphics_api api = graphics_api_opengl);
		static RenderingDevice* get_instance();
		static void shutdown_instance();

		virtual void initilize() = 0;
		virtual void begin_frame() = 0;
		virtual void draw(RenderObject* object, Shader* shader) = 0;

		virtual RenderObject* create_object(Mesh*) = 0;
		virtual void compile_shader(Shader*) = 0;

		virtual void set_shader_float(Shader*, const char* name, float v) = 0;
		virtual void set_shader_int(Shader*, const char* name, int v) = 0;
		virtual void set_shader_vec2(Shader*, const char* name, glm::vec2 v) = 0;
		virtual void set_shader_vec3(Shader*, const char* name, glm::vec3 v) = 0;
		virtual void set_shader_vec4(Shader*, const char* name, glm::vec4 v) = 0;
		virtual void set_shader_mat4(Shader*, const char* name, glm::mat4 v) = 0;

		virtual void shutdown() = 0;
		virtual void shutdown_object(RenderObject* object) = 0;
		virtual void shutdown_shader(Shader* shader) = 0;
	};
}

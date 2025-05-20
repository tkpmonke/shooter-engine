#pragma once

#include <GL/glew.h>

#define GLM_FORCE_CTOR_INIT
#include "rendering/RenderingDevice.hpp"

namespace engine::rendering::gl {

	struct GLRenderObject {
		unsigned int vao, vbo, ebo, indice_count;
	};

	class GLRenderingDevice : public RenderingDevice {
	public:
		void initilize();
		void begin_frame();
		void draw(RenderObject* object, Shader*);

		RenderObject* create_object(Mesh*);
		void compile_shader(Shader*);
		void bind_shader(Shader*);

		void set_shader_float(Shader*, const char* name, float v);
		void set_shader_int(Shader*, const char* name, int v);
		void set_shader_vec2(Shader*, const char* name, glm::vec2 v);
		void set_shader_vec3(Shader*, const char* name, glm::vec3 v);
		void set_shader_vec4(Shader*, const char* name, glm::vec4 v);
		void set_shader_mat4(Shader*, const char* name, glm::mat4 v);

		void shutdown();
		void shutdown_object(RenderObject* object);
		void shutdown_shader(Shader* shader);

	};
}

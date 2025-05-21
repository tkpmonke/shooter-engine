#pragma once

#include <GL/glew.h>

#define GLM_FORCE_CTOR_INIT
#include "rendering/RenderingDevice.hpp"
#include "crossplatform/Export.hpp"

namespace engine::rendering::gl {

	struct GLRenderObject {
		unsigned int vao, vbo, ebo, indice_count;
	};

	class GLRenderingDevice : public RenderingDevice {
	public:
		ENGINE_CROSSPLATFORM_EXPORT void initilize();
		ENGINE_CROSSPLATFORM_EXPORT void begin_frame();
		ENGINE_CROSSPLATFORM_EXPORT void draw(RenderObject* object, Shader*);

		ENGINE_CROSSPLATFORM_EXPORT RenderObject* create_object(Mesh*);
		ENGINE_CROSSPLATFORM_EXPORT void compile_shader(Shader*);
		ENGINE_CROSSPLATFORM_EXPORT void bind_shader(Shader*);

		ENGINE_CROSSPLATFORM_EXPORT void set_shader_float(Shader*, const char* name, float v);
		ENGINE_CROSSPLATFORM_EXPORT void set_shader_int(Shader*, const char* name, int v);
		ENGINE_CROSSPLATFORM_EXPORT void set_shader_vec2(Shader*, const char* name, glm::vec2 v);
		ENGINE_CROSSPLATFORM_EXPORT void set_shader_vec3(Shader*, const char* name, glm::vec3 v);
		ENGINE_CROSSPLATFORM_EXPORT void set_shader_vec4(Shader*, const char* name, glm::vec4 v);
		ENGINE_CROSSPLATFORM_EXPORT void set_shader_mat4(Shader*, const char* name, glm::mat4 v);

		ENGINE_CROSSPLATFORM_EXPORT void shutdown();
		ENGINE_CROSSPLATFORM_EXPORT void shutdown_object(RenderObject* object);
		ENGINE_CROSSPLATFORM_EXPORT void shutdown_shader(Shader* shader);

	};
}

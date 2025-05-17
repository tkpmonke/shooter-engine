#pragma once

#include <GL/glew.h>

#include "rendering/RenderingDevice.hpp"

namespace engine::rendering::gl {

	struct GLRenderObject {
		unsigned int vao, vbo, ebo, program, indice_count;
	};

	class GLRenderingDevice : public RenderingDevice {
	public:
		void initilize();
		void begin_frame();
		void draw(RenderObject* object);

		RenderObject* create_object(Mesh*, Shader*);
		void compile_shader(Shader*);

		void shutdown();
		void shutdown_object(RenderObject* object);
		void shutdown_shader(Shader* shader);

	};
}

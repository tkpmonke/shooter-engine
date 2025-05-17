#pragma once

#include "Mesh.hpp"
#include "Shader.hpp"

namespace engine::rendering {
	typedef void RenderObject;
	class RenderingDevice {
	public:
		virtual void initilize() = 0;
		virtual void begin_frame() = 0;
		virtual void draw(RenderObject* object) = 0;

		virtual RenderObject* create_object(Mesh*, Shader*) = 0;
		virtual void compile_shader(Shader*) = 0;

		virtual void shutdown() = 0;
		virtual void shutdown_object(RenderObject* object) = 0;
		virtual void shutdown_shader(Shader* shader) = 0;
	};
}

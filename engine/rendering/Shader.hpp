#pragma once

namespace engine::rendering {
	class Shader {
	public:
		char* vertex_shader;
		char* fragment_shader;
		void* internal;
	};
}

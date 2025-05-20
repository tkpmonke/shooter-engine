#pragma once

#include <string>

namespace engine::rendering {
	class Shader {
	public:
		char* vertex_shader;
		char* fragment_shader;
		std::string name;
		void* internal;
	};
}

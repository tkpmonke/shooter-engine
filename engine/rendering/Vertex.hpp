#pragma once
#include <glm/glm.hpp>

namespace engine::rendering {
	class Vertex {
	public:
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec2 uv;
	};
};

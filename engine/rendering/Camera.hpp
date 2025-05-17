#pragma once
#include <glm/glm.hpp>

namespace engine::rendering {
	class Camera {
	public:
		float fov, min, max;

		glm::mat4 view, perspective;
	};
}

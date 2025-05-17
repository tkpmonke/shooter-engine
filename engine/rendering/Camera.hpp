#pragma once
#define GLM_FORCE_CTOR_INIT
#include <glm/glm.hpp>

#include "windowing/Window.hpp"

namespace engine::rendering {
	class Camera {
	public:
		float fov = 75, min = 0.1f, max = 100;

		glm::mat4 view, projection;

		glm::vec3 position = {0,0,0}, rotation = {0,0,0};
		glm::vec3 forward, right, up;

		enum {
			projection_mode_orthographic = 0,
			projection_mode_perspective = 1,
		} projection_mode = projection_mode_perspective;

		void process(engine::windowing::Window*);
	};
}

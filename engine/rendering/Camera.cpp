#include "Camera.hpp"
#include <glm/gtc/quaternion.hpp>

namespace engine::rendering {
	void Camera::process(engine::windowing::Window* window) {
		switch (this->projection_mode) {
			case(projection_mode_orthographic):
         	this->projection = glm::ortho(0.f, (float)window->width, 0.f, (float)window->height, this->min, this->max); 
				break;
			case(projection_mode_perspective):
			default:
         	this->projection = glm::perspective(glm::radians(this->fov), (float)window->width/(float)window->height, this->min, this->max);
				break;
		}

		glm::vec3 euler = this->rotation;
      euler = glm::radians(euler);

      float pitch = euler.y;
      float yaw = euler.x;
      this->forward.x = -sin(yaw) * cos(pitch);
      this->forward.y = sin(pitch);
      this->forward.z = -cos(yaw) * cos(pitch);
      this->forward = glm::normalize(this->forward);
      this->right = glm::normalize(glm::cross(this->forward, glm::vec3(0,1,0)));
      this->up = glm::normalize(glm::cross(this->right, this->forward));

		this->view = glm::lookAt(this->position, this->position + this->forward, this->up);
	}
}

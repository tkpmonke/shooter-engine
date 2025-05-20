#include "Object.hpp"
#include "rendering/Camera.hpp"

#include <glm/gtc/quaternion.hpp>

namespace engine {
	void Object::initlize(rendering::Mesh* mesh, rendering::Shader* shader) {
		rendering::RenderingDevice* renderer = rendering::RenderingDevice::get_instance();
		this->render_object = renderer->create_object(mesh);
		this->shader = shader;
	}

	void Object::render(rendering::Camera* camera) {
		rendering::RenderingDevice* renderer = rendering::RenderingDevice::get_instance();
		
		glm::mat4 model = glm::mat4(1.f);

      model = glm::translate(model, this->position);
      model *= glm::mat4_cast(glm::quat(glm::radians(this->rotation)));
      model = glm::scale(model, this->scale);

		renderer->set_shader_mat4(this->shader, "u_model", model);
		renderer->set_shader_mat4(this->shader, "u_view", camera->view);
		renderer->set_shader_mat4(this->shader, "u_projection", camera->projection);

		renderer->draw(this->render_object, this->shader);
	}

	void Object::shutdown() {
		rendering::RenderingDevice* renderer = rendering::RenderingDevice::get_instance();
		renderer->shutdown_object(this->render_object);
	}
}

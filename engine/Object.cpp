#include "Object.hpp"
#include "Scene.hpp"
#include "rendering/Camera.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/matrix_decompose.hpp>

#include <iostream>
namespace engine {
	void Object::initilize(rendering::Mesh* mesh, rendering::Shader* shader) {
		rendering::RenderingDevice* renderer = rendering::RenderingDevice::get_instance();

		this->mesh_name = mesh->path;

		this->render_object = renderer->create_object(mesh);
		this->shader = shader;
	}

	void Object::calculate_transform() {

		glm::mat4 local = glm::mat4(1.f);
      local = glm::translate(local, this->position);
      local *= glm::mat4_cast(glm::quat(glm::radians(this->rotation)));
      local = glm::scale(local, this->scale);
		
		if (this->parent) {
			this->model_matrix = this->parent->model_matrix * local;
		} else {
			this->model_matrix = local;
		}

		glm::vec3 skew;
		glm::vec4 perspective;
		glm::quat rot_quat;
		glm::decompose(this->model_matrix, this->global_scale, rot_quat, this->global_position, skew, perspective);
		this->global_rotation = glm::degrees(glm::eulerAngles(rot_quat));

		for (Object& o : this->children) {
			o.parent = this;
			o.calculate_transform();
		}
	}

	void Object::render(rendering::Camera* camera) {
		rendering::RenderingDevice* renderer = rendering::RenderingDevice::get_instance();
		
		renderer->set_shader_mat4(this->shader, "u_model", model_matrix);
		renderer->set_shader_mat4(this->shader, "u_view", camera->view);
		renderer->set_shader_mat4(this->shader, "u_projection", camera->projection);

		renderer->draw(this->render_object, this->shader);
	}

	void Object::shutdown() {
		rendering::RenderingDevice* renderer = rendering::RenderingDevice::get_instance();
		renderer->shutdown_object(this->render_object);
	}
}

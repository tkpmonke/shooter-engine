#include "Object.hpp"
#include "Scene.hpp"
#include "rendering/Camera.hpp"

#include <glm/gtc/quaternion.hpp>
#include <iostream>
namespace engine {
	void Object::initilize(rendering::Mesh* mesh, rendering::Shader* shader) {
		rendering::RenderingDevice* renderer = rendering::RenderingDevice::get_instance();

		this->mesh_name = mesh->path;

		this->render_object = renderer->create_object(mesh);
		this->shader = shader;
	}

	void Object::calculate_transform() {
		if (this->parent) {
			this->global_position = this->position + this->parent->global_position;
		} else {
			this->global_position = this->position;
		}

		if (this->name == "Sphere3") {
			std::cout << this->global_position.x << ", " << this->global_position.y << ", " << this->global_position.z << "\n";

			std::cout << "Parent is " << (this->parent == nullptr ? "NULL\n" : "Real\n"); 
			std::cout << "Parent >> " << this->parent->global_position.x << ", " << this->parent->global_position.y << ", " << this->parent->global_position.z << "\n";
		}

		this->model_matrix = glm::mat4(1.f);
      model_matrix = glm::translate(model_matrix, this->global_position);
      model_matrix *= glm::mat4_cast(glm::quat(glm::radians(this->rotation)));
      model_matrix = glm::scale(model_matrix, this->scale);

		for (Object& o : this->children) {
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

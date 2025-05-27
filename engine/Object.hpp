#pragma once
#include "rendering/RenderingDevice.hpp"
#include "rendering/Camera.hpp"
#include "crossplatform/Export.hpp"

#define GLM_FORCE_CTOR_INIT
#include <glm/glm.hpp>

#include <string>

namespace engine {
	class Object {
	public:
		std::string name = "Object";
		unsigned long int id;

		std::vector<Object> children;
		Object* parent = NULL;

		glm::vec3 position = {0,0,0}, rotation = {0,0,0}, scale = {1,1,1};
		glm::vec3 forward = {0,0,0}, right = {0,0,0}, up = {0,0,0};

		std::string mesh_name;
		rendering::RenderObject* render_object;
		rendering::Shader* shader;

		ENGINE_CROSSPLATFORM_EXPORT void initilize(rendering::Mesh* mesh, rendering::Shader* shader);
		ENGINE_CROSSPLATFORM_EXPORT void calculate_transform();
		ENGINE_CROSSPLATFORM_EXPORT void render(rendering::Camera* camera);
		ENGINE_CROSSPLATFORM_EXPORT void shutdown();
		
		glm::vec3 global_position = {0, 0, 0};

		glm::mat4 model_matrix = {0};
	};
}

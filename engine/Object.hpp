#pragma once
#include "rendering/RenderingDevice.hpp"
#include "rendering/Camera.hpp"

#define GLM_FORCE_CTOR_INIT
#include <glm/glm.hpp>

#include <string>

namespace engine {
	class Object {
	public:
		std::string name = "Object";
		unsigned long int id;
		std::vector<Object> children;

		glm::vec3 position = {0,0,0}, rotation = {0,0,0}, scale = {1,1,1};
		glm::vec3 forward, right, up;

		rendering::RenderObject* render_object;
		rendering::Shader* shader;

		void initlize(rendering::Mesh* mesh, rendering::Shader* shader);
		void render(rendering::Camera* camera);
		void shutdown();
	};
}

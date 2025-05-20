#pragma once
#include "Object.hpp"
#include <vector>

namespace engine {
	class Scene {
	public:
		char* name = (char*)"Cool And Awesome Scene";

		std::vector<Object> objects;
		rendering::Camera camera;

		Object* create_object();

		void render_objects();

		void shutdown();

		static Scene& get_instance();
	};
}

#pragma once
#include "Object.hpp"
#include <vector>

#include "crossplatform/Export.hpp"

namespace engine {
	class Scene {
	public:
		std::string name = "Cool And Awesome Scene";

		std::vector<Object> objects;
		rendering::Camera camera;

		ENGINE_CROSSPLATFORM_EXPORT Object* create_object();
		
		ENGINE_CROSSPLATFORM_EXPORT void render_objects();
		
		ENGINE_CROSSPLATFORM_EXPORT void shutdown();
		
		ENGINE_CROSSPLATFORM_EXPORT static Scene& get_instance();
	};
}

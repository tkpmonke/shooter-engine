#include "Scene.hpp"

namespace engine {
	Object* Scene::create_object() {
		Object o;
		o.id = this->objects.size();
		this->objects.push_back(o);
		return &this->objects[this->objects.size()-1];
	}

	void Scene::render_objects() {
		for (Object& o : this->objects) {
			o.render(&this->camera);
		}
	}

	void Scene::shutdown() {
		for (Object& o : this->objects) {
			o.shutdown();
		}

		this->objects.clear();
	}
	
	Scene& Scene::get_instance() {
		static Scene scene;
		return scene;
	}
}

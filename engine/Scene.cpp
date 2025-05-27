#include "Scene.hpp"

#include "Logging.hpp"

namespace engine {
	Object* Scene::create_object() {
		Object o;
		o.id = this->objects.size();
		this->objects.push_back(o);
		return &this->objects[this->objects.size()-1];
	}

	void render_children(Object& o, Scene* s) {
		for (Object& c : o.children) {
			c.render(&s->camera);
			render_children(c, s);
		}
	}

	void Scene::render_objects() {
		for (engine::Object& o : engine::Scene::get_instance().objects) {
			o.calculate_transform();
		}
		for (Object& o : this->objects) {
			o.render(&this->camera);
			render_children(o, this);
		}
	}

	void shutdown_children(Object& o) {
		for (Object& c : o.children) {
			c.shutdown();
			shutdown_children(c);
		}
	}

	void Scene::shutdown() {
		for (Object& o : this->objects) {
			o.shutdown();
			shutdown_children(o);
		}

		this->objects.clear();
	}
	
	Scene& Scene::get_instance() {
		static Scene scene;
		return scene;
	}
}

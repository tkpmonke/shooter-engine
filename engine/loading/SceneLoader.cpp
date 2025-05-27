#include "SceneLoader.hpp"
#include "AssetCache.hpp"
#include "Scene.hpp"

#include "rendering/RenderingDevice.hpp"

#include <yaml-cpp/yaml.h>

/// literally the example on yaml-cpp's wiki but
/// for glm
namespace YAML {
	template<>
	struct convert<glm::vec3> {
	  static Node encode(const glm::vec3& rhs) {
	    Node node;
	    node.push_back(rhs.x);
	    node.push_back(rhs.y);
	    node.push_back(rhs.z);
	    return node;
	  }
	
	  static bool decode(const Node& node, glm::vec3& rhs) {
	    if(!node.IsSequence() || node.size() != 3) {
	      return false;
	    }
	
	    rhs.x = node[0].as<float>();
	    rhs.y = node[1].as<float>();
	    rhs.z = node[2].as<float>();
	    return true;
	  }
	};
}

namespace engine::loading {
	void handle_object(YAML::Node node, Object* obj) {

		obj->name = (node)["name"].as<std::string>();
		obj->position = (node)["position"].as<glm::vec3>();
		obj->rotation = (node)["rotation"].as<glm::vec3>();
		obj->scale = (node)["scale"].as<glm::vec3>();
		obj->mesh_name = (node)["mesh"].as<std::string>();

		AssetCache::get_instance().load_mesh(obj->mesh_name.data());

		rendering::RenderingDevice::draw_mode draw_mode = (rendering::RenderingDevice::draw_mode)(node)["draw_mode"].as<int>();
		rendering::Mesh* mesh = &AssetCache::get_instance().meshes[obj->mesh_name.data()];

		obj->render_object = rendering::RenderingDevice::get_instance()->create_object(mesh, draw_mode);

		obj->shader = &AssetCache::get_instance().shaders[((node)["shader"].as<std::string>().data())];
	}

	void handle_children(YAML::Node node, Object* obj) {	
		for (size_t i = 0; i < (node)["children"].size(); ++i) {
			obj->children.push_back({});
			Object* o = &obj->children[obj->children.size()-1];
			handle_object((node["children"][i]), o);
		}
	}

	void load_scene(const char* path) {
		const std::string yaml_data = AssetCache::get_instance().load_file(path);
		YAML::Node root = YAML::Load(yaml_data)["scene"];

		engine::Scene& s = Scene::get_instance();

		s.name = root["name"].as<std::string>();
		for (size_t i = 0; i < root["objects"].size(); ++i) {
			Object* obj = s.create_object();
			handle_object((root["objects"][i]), obj);
			handle_children(root["objects"][i], obj);
		}
	}
}

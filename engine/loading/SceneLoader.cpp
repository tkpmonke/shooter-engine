#include "SceneLoader.hpp"
#include "AssetCache.hpp"
#include "Scene.hpp"

#include "rendering/RenderingDevice.hpp"

#include <yaml-cpp/yaml.h>
#include <iostream>

/// the example on yaml-cpp's wiki but
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

	Emitter& operator << (Emitter& out, const glm::vec3& vec) {
		out << YAML::Flow;
		out << YAML::BeginSeq << vec.x << vec.y << vec.z << YAML::EndSeq;
		return out;
	}
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
			Object* o = &obj->children.back();
			o->parent = obj;
			handle_object(node["children"][i], o);
			handle_children(node["children"][i], o);
		}
	}

	void load_scene(const char* path) {
		const std::string yaml_data = AssetCache::get_instance().load_file(path);
		YAML::Node root = YAML::Load(yaml_data)["scene"];

		engine::Scene& s = Scene::get_instance();

		s.name = root["name"].as<std::string>();
		s.path = path;
		for (size_t i = 0; i < root["objects"].size(); ++i) {
			Object* obj = s.create_object();
			handle_object((root["objects"][i]), obj);
			handle_children(root["objects"][i], obj);
		}
	}

	void save_object(YAML::Emitter* out, Object* o) {
		*out << YAML::BeginMap;
	
		*out << YAML::Key << "name";
		*out << YAML::Value << o->name;
		*out << YAML::Key << "position";
		*out << YAML::Value << o->position;
		*out << YAML::Key << "rotation";
		*out << YAML::Value << o->rotation;
		*out << YAML::Key << "scale";
		*out << YAML::Value << o->scale;
		*out << YAML::Key << "mesh";
		*out << YAML::Value << o->mesh_name;
		*out << YAML::Key << "draw_mode";
		*out << YAML::Value << 0;
		*out << YAML::Key << "shader";
		*out << YAML::Value << o->shader->name;
		*out << YAML::Key << "children";

		if (o->children.empty()) {
			*out << YAML::Flow;
			*out << YAML::BeginSeq << YAML::EndSeq;
		} else {
			*out << YAML::BeginSeq;
			for (size_t i = 0; i < o->children.size(); ++i) {
				save_object(out, &o->children[i]);
			}
			*out << YAML::EndSeq;
		}

		*out << YAML::EndMap;
	}

	void save_scene() {
		YAML::Emitter out;
		engine::Scene& s = Scene::get_instance();

		out << YAML::BeginMap;
		
		out << YAML::Key << "scene";
		out << YAML::Value << YAML::BeginMap;

		out << YAML::Key << "name";
		out << YAML::Value << s.name;

		out << YAML::Key << "objects";
		out << YAML::Value << YAML::BeginSeq;
		for (size_t i = 0; i < s.objects.size(); ++i) {
			save_object(&out, &s.objects[i]);
		}
		out << YAML::EndSeq;

		out << YAML::Key << "camera";
		out << YAML::Value << YAML::BeginMap;
		
		out << YAML::Key << "fov";
		out << YAML::Value << s.camera.fov;
		out << YAML::Key << "min";
		out << YAML::Value << s.camera.min;
		out << YAML::Key << "max";
		out << YAML::Value << s.camera.max;
		out << YAML::Key << "position";
		out << YAML::Value << s.camera.position;
		out << YAML::Key << "rotation";
		out << YAML::Value << s.camera.rotation;
		out << YAML::Key << "projection";
		out << YAML::Value << s.camera.projection_mode;

		out << YAML::EndMap;

		AssetCache::get_instance().write_file(s.path.data(), out.c_str());
	}
}

#pragma once

#include "rendering/Mesh.hpp"
#include "rendering/Shader.hpp"
#include "crossplatform/Export.hpp"

#include <unordered_map>
#include <map>
#include <string>

namespace engine::loading {
	class AssetCache {
	public:
		enum load_mode {
			
			/// used as the default, used to make sure that set_mode
			/// has been called
			load_mode_none = 0,
			
			/// load from a .wad file (same format as idtech 1 :3)
			/// used for compiled games
			load_mode_wad,

			/// load recursively from a folder
			/// used for editor
			/// this mode also allows for dynamic file reloading
			load_mode_folder
		};

		struct file_info {
			struct {
				size_t location;
				size_t size;
			} wad;

			struct {
				std::string data;
			} folder;

			bool is_loaded;
		};

		std::unordered_map<std::string, rendering::Mesh> meshes;
		
		std::map<std::string, rendering::Shader> shaders;

		std::unordered_map<std::string, file_info> files;

		ENGINE_CROSSPLATFORM_EXPORT const std::string load_file(const char* path);
		ENGINE_CROSSPLATFORM_EXPORT void load_mesh(const char* path);
		ENGINE_CROSSPLATFORM_EXPORT void load_shader(const char* vertex_path, const char* fragment_path, const char* shader_path);

		ENGINE_CROSSPLATFORM_EXPORT void write_file(const char* path, std::string data);

		ENGINE_CROSSPLATFORM_EXPORT load_mode get_mode();
		ENGINE_CROSSPLATFORM_EXPORT void set_mode(load_mode mode);

		ENGINE_CROSSPLATFORM_EXPORT static AssetCache& get_instance();
		ENGINE_CROSSPLATFORM_EXPORT static void shutdown();
		
	private:
		load_mode mode = load_mode_none;

		void read_file(const char* path);

		char* wad_file;
	};
}

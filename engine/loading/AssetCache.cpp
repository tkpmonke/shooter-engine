#include "AssetCache.hpp"

#include "Logging.hpp"
#include "MeshLoader.hpp"
#include "rendering/RenderingDevice.hpp"

#include <filesystem>
#include <fstream>

#include <stdlib.h>

namespace engine::loading {

	const std::string AssetCache::load_file(const char* path) {
		switch (this->mode) {
			case (load_mode_none):
				logging::print_error("load mode is none, please call set_mode(load_mode mode) before loading files");
				break;
			case (load_mode_wad):
				if (this->files.count(path) == 1) {
					char* location = this->wad_file + this->files[path].wad.location;
					const std::string out(location, this->files[path].wad.size);
					return out;
				} else {
					logging::print_error(((std::string)"File not found : " + path).data());
				}
				break;
			case (load_mode_folder):
				if (std::filesystem::exists((std::filesystem::current_path() / path).make_preferred())) {
					
					if (!this->files[path].is_loaded) {
						read_file(path);
					}

					return this->files[path].folder.data;
				
				} else {
					logging::print_error(((std::string)"File not found : " + path).data());
				}
		}
		return "";
	}
	
	void AssetCache::load_mesh(const char* path) {
		switch (this->mode) {
			case (load_mode_none):
				logging::print_error("load mode is none, please call set_mode(load_mode mode) before loading files");
				break;
			case (load_mode_wad):
				if (this->files.count(path) == 1) {
					char* location = this->wad_file + this->files[path].wad.location;

					std::string file_ext = ((std::filesystem::path)this->files[path].folder.data).extension().string();
					this->meshes[path] = (*loading::load_mesh(location,
								(file_ext == ".glb" ? mesh_type_binary : mesh_type_ascii) | mesh_type_memory,
								this->files[path].wad.size));
					this->meshes[path].path = path;
				} else {
					logging::print_error(((std::string)"File not found : " + path).data());
				}
				break;
			case (load_mode_folder):
				if (std::filesystem::exists(std::filesystem::current_path() / path)) {
					
					if (!this->files[path].is_loaded) {
						read_file(path);
					}

					std::string real_path = (std::filesystem::current_path() / ((std::filesystem::path)path).make_preferred()).string();

					std::string file_ext = ((std::filesystem::path)path).extension().string();
					this->meshes[path] = (*loading::load_mesh(real_path.data(),
								(file_ext == ".glb" ? mesh_type_binary : mesh_type_ascii) | mesh_type_file));

					this->meshes[path].path = path;
				} else {
					logging::print_error(((std::string)"File not found : " + (std::filesystem::current_path()/ ((std::filesystem::path)path).make_preferred()).string()).data());
				}
			default:
				break;
		}
	}
	
	void AssetCache::load_shader(const char* vertex_path, const char* fragment_path, const char* shader_path) {
		switch (this->mode) {
			case (load_mode_none):
				logging::print_error("load mode is none, please call set_mode(load_mode mode) before loading files");
				break;
			case (load_mode_wad):
				if (this->files.count(vertex_path) == 1 && this->files.count(fragment_path) == 1) {
					char* vert_location = this->wad_file + this->files[vertex_path].wad.location;
					char* frag_location = this->wad_file + this->files[fragment_path].wad.location;

					this->shaders[shader_path].vertex_shader = vert_location;
					this->shaders[shader_path].fragment_shader = frag_location;
					rendering::RenderingDevice::get_instance()->compile_shader(&this->shaders[shader_path]);
				} else {
					logging::print_error(((std::string)"File not found : " + vertex_path + (std::string)" and " + fragment_path).data());
				}
				break;
			case (load_mode_folder):
				if (std::filesystem::exists(std::filesystem::current_path() / ((std::filesystem::path)vertex_path).make_preferred())
						&& std::filesystem::exists(std::filesystem::current_path() / ((std::filesystem::path)fragment_path).make_preferred())) {

					read_file(vertex_path);
					read_file(fragment_path);

					this->shaders[shader_path].vertex_shader = this->files[vertex_path].folder.data.data();
					this->shaders[shader_path].fragment_shader = this->files[fragment_path].folder.data.data();
					this->shaders[shader_path].name = shader_path;
					rendering::RenderingDevice::get_instance()->compile_shader(&this->shaders[shader_path]);
				} else {
					logging::print_error(((std::string)"File not found : " + ((std::filesystem::path)vertex_path).make_preferred().string() + (std::string)" and " + ((std::filesystem::path)fragment_path).make_preferred().string()).data());
				}
				break;
			default:
				break;
		}
	}

	AssetCache::load_mode AssetCache::get_mode() {
		return this->mode;
	}

	void AssetCache::set_mode(AssetCache::load_mode mode) {
		if (this->mode == load_mode_none) {
			this->mode = mode;
		} else {
			logging::print_error("Load mode already set");
		}
	}

	AssetCache& AssetCache::get_instance() {
		static AssetCache instance;
		return instance;
	}

	void AssetCache::shutdown() {
		for (auto [name, s] : get_instance().shaders) {
			rendering::RenderingDevice::get_instance()->shutdown_shader(&s);
		}
	}

	void AssetCache::read_file(const char* path) {
		std::ifstream ifstream((std::filesystem::current_path() / path).make_preferred(), std::ios::binary);

		if (!ifstream.is_open()) {
			logging::print_error("Failed to open file");
			exit(2);
		}

		ifstream.seekg(0, std::ios::end);
		size_t size = ifstream.tellg();
		this->files[path].folder.data.resize(size);
		ifstream.seekg(0, std::ios::beg);
		ifstream.read(&this->files[path].folder.data[0], size);
		ifstream.close();

		this->files[path].is_loaded = true;
	}

	void AssetCache::write_file(const char* path, std::string data) {
		std::ofstream ofstream((std::filesystem::current_path() / path).make_preferred());
		ofstream << data;
		ofstream.close();
	}
}

#pragma once

#include "rendering/Mesh.hpp"
#include "crossplatform/Export.hpp"

namespace engine::loading {
	enum mesh_type {
		mesh_type_ascii = 1,
		mesh_type_binary = 2,
		mesh_type_file = 4,
		mesh_type_memory = 8
	};
	ENGINE_CROSSPLATFORM_EXPORT rendering::Mesh* load_mesh(const char* path, int type = (mesh_type_binary | mesh_type_file), size_t size = 0);
}

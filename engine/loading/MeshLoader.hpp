#pragma once

#include "rendering/Mesh.hpp"

namespace engine::loading {
	enum mesh_type {
		mesh_type_ascii = 0,
		mesh_type_binary = 1
	};
	rendering::Mesh* LoadMesh(const char* path, mesh_type type = mesh_type_binary);
}

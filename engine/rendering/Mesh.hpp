#pragma once

#include "Vertex.hpp"

#include <vector>
#include <string>

namespace engine::rendering {
	class Mesh {
	public:
		std::string path;
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;
	};
}

#pragma once

#include "Vertex.hpp"

#include <vector>

namespace engine::rendering {
	class Mesh {
	public:
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;
	};
}

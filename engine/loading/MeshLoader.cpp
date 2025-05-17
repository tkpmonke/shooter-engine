#include "MeshLoader.hpp"
#include "Logging.hpp"

#define TINYGLTF_IMPLEMENTATION
#define TINYGLTF_USE_CPP14
#define TINYGLTF_NO_EXTERNAL_IMAGE
#define TINYGLTF_NO_STB_IMAGE
#define TINYGLTF_NO_STB_IMAGE_WRITE
#include "tinygltf/tiny_gltf.h"

namespace engine::loading {
	void set_vec3_from_float3(const float* src, glm::vec3* dest) {
		dest->x = src[0];
		dest->y = src[1];
		dest->z = src[2];
	}
	
	void set_vec2_from_float2(const float* src, glm::vec2* dest) {
		dest->x = src[0];
		dest->y = src[1];
	}

	tinygltf::Model model;
	rendering::Mesh* LoadMesh(const char* path, mesh_type type) {
		rendering::Mesh* mesh = (rendering::Mesh*)malloc(sizeof(rendering::Mesh));
		memset((void*)mesh, 0, sizeof(rendering::Mesh));

		tinygltf::TinyGLTF loader;
		std::string err;
		std::string warn;
		
		bool ret;
		switch (type) {
			case (mesh_type_ascii):
				ret = loader.LoadASCIIFromFile(&model, &err, &warn, path);
				break;
			case (mesh_type_binary):
			default:
				ret = loader.LoadBinaryFromFile(&model, &err, &warn, path);
				break;
		}
		if (!warn.empty()) {
			engine::logging::print_warning(warn.c_str());
		} if (!err.empty()) {
			engine::logging::print_error(err.c_str());
		}

		if (!ret) {
			engine::logging::print_error(((std::string)"Failed to load GLTF file " + path).data());
		}

		if (model.meshes.empty()) {
			engine::logging::print_error(((std::string)"No Meshes Found in GLTF file " + path).data());
		}

		const tinygltf::Mesh& gmesh = model.meshes[0];
		const tinygltf::Primitive& prim = gmesh.primitives[0];

		const auto& accessor_pos = model.accessors[prim.attributes.at("POSITION")];
		const auto& bufferView_pos = model.bufferViews[accessor_pos.bufferView];
		const auto& buffer_pos = model.buffers[bufferView_pos.buffer];

		size_t vertex_count = accessor_pos.count;
		mesh->vertices.resize(vertex_count);

		const float* pos_data = reinterpret_cast<const float*>
			(&buffer_pos.data[accessor_pos.byteOffset + bufferView_pos.byteOffset]);
		
		// Normal (optional)
		const float* norm_data = nullptr;
		if (prim.attributes.count("NORMAL")) {
			const auto& accessor_norm = model.accessors[prim.attributes.at("NORMAL")];
		   const auto& bufferView_norm = model.bufferViews[accessor_norm.bufferView];
		   const auto& buffer_norm = model.buffers[bufferView_norm.buffer];
		   norm_data = reinterpret_cast<const float*>
				(&buffer_norm.data[accessor_norm.byteOffset + bufferView_norm.byteOffset]);
		}
		
		// Texcoord (optional)
		const float* uv_data = nullptr;
		if (prim.attributes.count("TEXCOORD_0")) {
		    const auto& accessor_uv = model.accessors[prim.attributes.at("TEXCOORD_0")];
		    const auto& bufferView_uv = model.bufferViews[accessor_uv.bufferView];
		    const auto& buffer_uv = model.buffers[bufferView_uv.buffer];
		    uv_data = reinterpret_cast<const float*>(
		        &buffer_uv.data[accessor_uv.byteOffset + bufferView_uv.byteOffset]);
		}

		for (size_t i = 0; i < vertex_count; ++i) {
			mesh->vertices[i].position.x = pos_data[i*3];
			mesh->vertices[i].position.y = pos_data[i*3+1];
			mesh->vertices[i].position.z = pos_data[i*3+2];
			
			if (norm_data) {
				set_vec3_from_float3(&norm_data[i*3], &mesh->vertices[i].normal);
			} if (uv_data) {
				set_vec2_from_float2(&uv_data[i*2], &mesh->vertices[i].uv);
			}
		}

		if (prim.indices >= 0) {
			const auto& accessor_idx = model.accessors[prim.indices];
			const auto& bufferView_idx = model.bufferViews[accessor_idx.bufferView];
			const auto& buffer_idx = model.buffers[bufferView_idx.buffer];

			const void* data_ptr = &buffer_idx.data[accessor_idx.byteOffset + bufferView_idx.byteOffset];
			mesh->indices.resize((unsigned int)accessor_idx.count);

			for (size_t i = 0; i < accessor_idx.count; ++i) {
				switch (accessor_idx.componentType) {
					case TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT:
						mesh->indices[i] = ((const uint16_t*)data_ptr)[i];
						break;
					case TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE:
						mesh->indices[i] = ((const uint8_t*)data_ptr)[i];
						break;
					case TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT:
						mesh->indices[i] = ((const uint32_t*)data_ptr)[i];
						break;
					default:
						return 0;
				}
			}
		}

		return mesh;
	}
}

#include "GUI.hpp"

#include "rendering/Mesh.hpp"
#include "loading/AssetCache.hpp"
#include "Scene.hpp"

#include <glm/gtc/quaternion.hpp>

using namespace engine::rendering;

namespace editor::gui {
	const static std::vector<Vertex> line_cube_vertices = {
		{
			{-1, -1, -1},
			{1, 1, 1},
			{0, 0}
		},
		{
			{1, -1, -1},
			{1, 1, 1},
			{0, 0}
		},
		{
			{1, 1, -1},
			{1, 1, 1},
			{0, 0}
		},
		{
			{-1, 1, -1},
			{1, 1, 1},
			{0, 0}
		},
		{
			{-1, -1, 1},
			{1, 1, 1},
			{0, 0}
		},
		{
			{1, -1, 1},
			{1, 1, 1},
			{0, 0}
		},
		{
			{1, 1, 1},
			{1, 1, 1},
			{0, 0}
		},
		{
			{-1, 1, 1},
			{1, 1, 1},
			{0, 0}
		}
	};

	const static std::vector<unsigned int> line_cube_indices = {
		0, 1,
		1, 2,
		2, 3,
		3, 0,

		4, 5,
		5, 6,
		6, 7,
		7, 4,
		
		0, 4,
		1, 5,
		2, 6,
		3, 7
	};

	const static Mesh line_cube = {
		.path = "",
		.vertices = line_cube_vertices,
		.indices = line_cube_indices,
	};

	void initilize_highlights() {
		GUIState& state = GUIState::get_instance();
		state.highlight_object = RenderingDevice::get_instance()->create_object((Mesh*)&line_cube, RenderingDevice::draw_mode_lines);
	}

	void draw_highlights() {
		GUIState& state = GUIState::get_instance();
		if (!state.highlight_status || !state.object_selected || state.selected_object == NULL) {
			return;
		}

		RenderingDevice* renderer = RenderingDevice::get_instance();
		engine::Object* obj = state.selected_object;
		Camera* camera = &state.camera;
		Shader* shader = &engine::loading::AssetCache::get_instance().shaders["core/default"];
		
		glm::mat4 model = glm::mat4(1.f);
      model = glm::translate(model, obj->position);
      model *= glm::mat4_cast(glm::quat(glm::radians(obj->rotation)));
      model = glm::scale(model, obj->scale);

		renderer->set_shader_mat4(shader, "u_model", model);
		renderer->set_shader_mat4(shader, "u_view", camera->view);
		renderer->set_shader_mat4(shader, "u_projection", camera->projection);

		renderer->set_line_width(5);
		renderer->draw(state.highlight_object, shader);
	}

	void shutdown_highlights() {
		GUIState& state = GUIState::get_instance();
		RenderingDevice::get_instance()->shutdown_object(state.highlight_object);
	}
}

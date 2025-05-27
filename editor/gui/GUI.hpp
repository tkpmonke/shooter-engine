#pragma once

#include "Object.hpp"
#include "windowing/Window.hpp"
#include "rendering/Camera.hpp"
#include "rendering/RenderingDevice.hpp"
#include "crossplatform/Export.hpp"

#include "imgui/imgui.h"
#include "imgui/imgui_stdlib.h"
#include "imguizmo/ImGuizmo.h"


namespace editor::gui {
	ENGINE_CROSSPLATFORM_EXPORT void initilize(engine::windowing::Window*);
	ENGINE_CROSSPLATFORM_EXPORT void begin_frame();
	ENGINE_CROSSPLATFORM_EXPORT void finish_frame();
	ENGINE_CROSSPLATFORM_EXPORT void shutdown();

	/// Editor Windows
	ENGINE_CROSSPLATFORM_EXPORT void draw_hierarchy();
	ENGINE_CROSSPLATFORM_EXPORT void draw_inspector();
	ENGINE_CROSSPLATFORM_EXPORT void draw_gizmos();

	/// draws all of the windows
	ENGINE_CROSSPLATFORM_EXPORT void draw_all();

	ENGINE_CROSSPLATFORM_EXPORT void raw_mouse_callback(RGFW_window* window, RGFW_point point, RGFW_point vector);
	ENGINE_CROSSPLATFORM_EXPORT void process_editor_camera();

	/// Hightlight selected object
	ENGINE_CROSSPLATFORM_EXPORT void initilize_highlights();
	ENGINE_CROSSPLATFORM_EXPORT void draw_highlights();
	ENGINE_CROSSPLATFORM_EXPORT void shutdown_highlights();

	class GUIState {
	public:
		bool object_selected = false;
		engine::Object* selected_object;

		ImGuizmo::OPERATION operation = ImGuizmo::OPERATION::TRANSLATE;
		ImGuizmo::MODE mode = ImGuizmo::MODE::LOCAL;

		engine::rendering::Camera camera;
		engine::windowing::Window* window;

		RGFW_point mouse;

		float camera_sens = 0.025f;
		float camera_speed = 5.f;
		float shift_mul = 2;

		bool highlight_status = true;
		engine::rendering::RenderObject* highlight_object;

		ENGINE_CROSSPLATFORM_EXPORT static GUIState& get_instance();
	};
}

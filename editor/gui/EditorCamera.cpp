#include "GUI.hpp"

using namespace engine;
using namespace engine::rendering;
using namespace engine::windowing;

#include <iostream>
namespace editor::gui {
	void raw_mouse_callback(RGFW_window* window, RGFW_point point, RGFW_point vector) {
		(void)point;
		(void)window;
		GUIState::get_instance().mouse = { GUIState::get_instance().mouse.x + vector.x, GUIState::get_instance().mouse.y + vector.y };
	}
	
	void process_editor_camera() {
		GUIState& state = GUIState::get_instance();
		Camera* camera = &state.camera;

		static RGFW_point prev_mouse;
		double offx, offy;

		static float prtime;
		float time = state.window->time;
		float dt = time-prtime;
		prtime = time;

		offx = state.mouse.x - prev_mouse.x;
		offy = prev_mouse.y - state.mouse.y;

		prev_mouse = state.mouse;

		if (RGFW_isMousePressed(state.window->rgfw_window, RGFW_mouseRight)) {
			RGFW_window_mouseUnhold(state.window->rgfw_window);

			RGFW_window_mouseHold(state.window->rgfw_window, RGFW_AREA(state.window->width/2, state.window->height/2));
			ImGui::SetMouseCursor(ImGuiMouseCursor_None);

			camera->rotation.x += -offx * state.camera_sens;
			camera->rotation.y += offy * state.camera_sens;

			if (RGFW_isPressed(state.window->rgfw_window, RGFW_shiftL)) {
				dt *= state.shift_mul;
			}

			if (RGFW_isPressed(state.window->rgfw_window, RGFW_w)) { 
				camera->position += state.camera_speed * camera->forward * dt;
			} if (RGFW_isPressed(state.window->rgfw_window, RGFW_a)) {
				camera->position += state.camera_speed * -camera->right * dt;
			} if (RGFW_isPressed(state.window->rgfw_window, RGFW_s)) {
				camera->position += state.camera_speed * -camera->forward * dt;
			} if (RGFW_isPressed(state.window->rgfw_window, RGFW_d)) {
				camera->position += state.camera_speed * camera->right * dt;
			} if (RGFW_isPressed(state.window->rgfw_window, RGFW_e)) {
				camera->position += state.camera_speed * camera->up * ((float)dt/1.5f);
			} if (RGFW_isPressed(state.window->rgfw_window, RGFW_q)) {
				camera->position += state.camera_speed * -camera->up * ((float)dt/1.5f);
			}

		} else {
			ImGui::SetMouseCursor(ImGuiMouseCursor_Arrow);
			RGFW_window_mouseUnhold(state.window->rgfw_window);
		}

		camera->rotation.y = glm::clamp(camera->rotation.y, -89.f, 89.f);
		camera->process(state.window);
	}
}

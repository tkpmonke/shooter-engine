#include "GUI.hpp"
#include "Scene.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/matrix_decompose.hpp>

namespace editor::gui {
	void draw_gizmos() {
		GUIState& gui_state = GUIState::get_instance();
		if (gui_state.selected_object == NULL || gui_state.object_selected == false) return;
		
		engine::Scene& scene = engine::Scene::get_instance();
		
		ImGuizmo::SetRect(0, 0, gui_state.window->width, gui_state.window->height);
		ImGuizmo::SetOrthographic(scene.camera.projection_mode == engine::rendering::Camera::projection_mode_orthographic);

		if (ImGuizmo::Manipulate(
			glm::value_ptr(gui_state.camera.view),
			glm::value_ptr(gui_state.camera.projection),
			gui_state.operation,
			gui_state.mode,
			glm::value_ptr(gui_state.selected_object->model_matrix))
		) {

			ImGuizmo::DecomposeMatrixToComponents(
				glm::value_ptr(gui_state.selected_object->model_matrix),
				glm::value_ptr(gui_state.selected_object->global_position),
				glm::value_ptr(gui_state.selected_object->global_rotation),
				glm::value_ptr(gui_state.selected_object->global_scale)
			);

			if (gui_state.selected_object->parent == NULL) {
				gui_state.selected_object->position = gui_state.selected_object->global_position;
				gui_state.selected_object->rotation = gui_state.selected_object->global_rotation;
				gui_state.selected_object->scale = gui_state.selected_object->global_scale;;
			} else {
				glm::mat4 parent_inverse = glm::inverse(gui_state.selected_object->parent->model_matrix);
				glm::mat4 local = parent_inverse * gui_state.selected_object->model_matrix;
				
				glm::vec3 skew;
				glm::vec4 perspective;
				glm::quat rot_quat;
				
				glm::decompose(local,
					gui_state.selected_object->scale,
					rot_quat,
					gui_state.selected_object->position,
					skew,
					perspective);
				
				gui_state.selected_object->rotation = glm::degrees(glm::eulerAngles(rot_quat));
			}
		}


		if (ImGui::Begin("Tools")) {
			if (ImGui::RadioButton("Translate", gui_state.operation == ImGuizmo::TRANSLATE)) {
        		gui_state.operation = ImGuizmo::TRANSLATE;
			}
    		ImGui::SameLine();
    		if (ImGui::RadioButton("Rotate", gui_state.operation == ImGuizmo::ROTATE)) {
        		gui_state.operation = ImGuizmo::ROTATE;
			}
    		ImGui::SameLine();
    		if (ImGui::RadioButton("Scale", gui_state.operation == ImGuizmo::SCALE)) {
       		gui_state.operation = ImGuizmo::SCALE;
			}

			if (gui_state.operation != ImGuizmo::SCALE) {
				if (ImGui::RadioButton("Local", gui_state.mode == ImGuizmo::LOCAL)) {
        			gui_state.mode = ImGuizmo::LOCAL;
				}
    			ImGui::SameLine();
    			if (ImGui::RadioButton("World", gui_state.mode == ImGuizmo::WORLD)) {
        			gui_state.mode = ImGuizmo::WORLD;
				}
			}
		}
		ImGui::End();
	}
}

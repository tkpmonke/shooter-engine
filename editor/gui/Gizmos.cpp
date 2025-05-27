#include "GUI.hpp"
#include "Scene.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace editor::gui {
	void draw_gizmos() {
		GUIState& gui_state = GUIState::get_instance();
		if (gui_state.selected_object == NULL || gui_state.object_selected == false) return;
		
		engine::Scene& scene = engine::Scene::get_instance();
		
		ImGuizmo::SetRect(0, 0, gui_state.window->width, gui_state.window->height);
		ImGuizmo::SetOrthographic(scene.camera.projection_mode == engine::rendering::Camera::projection_mode_orthographic);

		glm::mat4 model = glm::mat4(1.f);
      model = glm::translate(model, gui_state.selected_object->position);
      model *= glm::mat4_cast(glm::quat(glm::radians(gui_state.selected_object->rotation)));
      model = glm::scale(model, gui_state.selected_object->scale);
		
		if (ImGuizmo::Manipulate(
			glm::value_ptr(gui_state.camera.view),
			glm::value_ptr(gui_state.camera.projection),
			gui_state.operation,
			gui_state.mode,
			glm::value_ptr(model))
		) {
			
			ImGuizmo::DecomposeMatrixToComponents(
				glm::value_ptr(model),
				glm::value_ptr(gui_state.selected_object->position),
				glm::value_ptr(gui_state.selected_object->rotation),
				glm::value_ptr(gui_state.selected_object->scale)
			);
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

#include "GUI.hpp"
#include "Scene.hpp"
#include "loading/AssetCache.hpp"
#include <glm/gtc/type_ptr.hpp>

namespace editor::gui {
	void draw_shader_selector() {	
		ImGui::SetNextWindowPos(ImGui::GetMainViewport()->GetCenter(), ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
		if (ImGui::BeginPopupModal("Set Shader", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
			ImGui::Text("Select the shader to be used");

			static engine::rendering::Shader* selected;
			if (ImGui::BeginListBox("##Shaders")) {
				for (const auto& [name, shader] : engine::loading::AssetCache::get_instance().shaders) {
					if (ImGui::Selectable(name.data(), selected == &shader)) {
						selected = (engine::rendering::Shader*)&shader;
					}

					if (selected == &shader) {
						ImGui::SetItemDefaultFocus();
					}
				}
				ImGui::EndListBox();
			}			
			
			if (ImGui::Button("Close")) {
				ImGui::CloseCurrentPopup();
			}

			ImGui::SameLine();
			
			if (ImGui::Button("Confirm")) {
				GUIState::get_instance().selected_object->shader = selected;
				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
		}
	}

	void draw_inspector() {
		if (ImGui::Begin("Inspector")) {
			engine::Scene& scene = engine::Scene::get_instance();

			if (GUIState::get_instance().selected_object == NULL && GUIState::get_instance().object_selected) {
				/// Camera Selected
				ImGui::Text("Camera");

				ImGui::SeparatorText("Transform");

				ImGui::DragFloat3("Position", glm::value_ptr(scene.camera.position));
				ImGui::DragFloat3("Rotation", glm::value_ptr(scene.camera.rotation));
				
				ImGui::SeparatorText("Camera");
				
				ImGui::DragFloat("FOV", &scene.camera.fov);
				ImGui::DragFloat("Min", &scene.camera.min);
				ImGui::DragFloat("Max", &scene.camera.max);

				if (ImGui::RadioButton("Perspective", 
						scene.camera.projection_mode == engine::rendering::Camera::projection_mode_perspective)) {
					scene.camera.projection_mode = engine::rendering::Camera::projection_mode_perspective;
				} 
				ImGui::SameLine();
				if (ImGui::RadioButton("Orthographic", 
						scene.camera.projection_mode == engine::rendering::Camera::projection_mode_orthographic)) {
					scene.camera.projection_mode = engine::rendering::Camera::projection_mode_orthographic;
				}
			} else if (GUIState::get_instance().object_selected) {
				/// Object Selected
				engine::Object* obj = GUIState::get_instance().selected_object;
				
				ImGui::InputText("Name", &obj->name);
				ImGui::Text("ID : %lu", obj->id);

				ImGui::SeparatorText("Transform");

				ImGui::DragFloat3("Position", glm::value_ptr(obj->position));
				ImGui::DragFloat3("Rotation", glm::value_ptr(obj->rotation));
				ImGui::DragFloat3("Scale", glm::value_ptr(obj->scale));

				ImGui::SeparatorText("Material");

				ImGui::Text("%s", ((std::string)"Current Shader > " + obj->shader->name).c_str());
				if (ImGui::Button("Set Current Shader")) {
					ImGui::OpenPopup("Set Shader");
				}

				ImGui::SeparatorText("Mesh");
				ImGui::Text("%s", ((std::string)"Current Mesh > " + obj->mesh_name).c_str());
			}
		}

		draw_shader_selector();

		ImGui::End();
	}
}

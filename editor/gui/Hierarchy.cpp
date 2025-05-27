#include "GUI.hpp"
#include "Object.hpp"
#include "Scene.hpp"

namespace editor::gui {
	void draw_object_node(engine::Object& o) {
		ImGuiTreeNodeFlags is_leaf = o.children.empty() ? ImGuiTreeNodeFlags_Leaf : 0;
		if (ImGui::TreeNodeEx(o.name.data(), ImGuiTreeNodeFlags_DefaultOpen | is_leaf)) {
			
			if (ImGui::IsItemClicked()) {
				GUIState::get_instance().object_selected = true;
				GUIState::get_instance().selected_object = &o;
			}

			for (engine::Object& child : o.children) {
				draw_object_node(child);
			}

			ImGui::TreePop();
		}
	}

	void draw_hierarchy() {
		if (ImGui::Begin("Scene Hierarchy")) {
			engine::Scene& scene = engine::Scene::get_instance();

			ImGuiTreeNodeFlags flag = ImGuiTreeNodeFlags_DefaultOpen;
			if (ImGui::TreeNodeEx(scene.name.data(), flag)) {

				if (ImGui::TreeNodeEx("Camera", flag | ImGuiTreeNodeFlags_Leaf)) {
					if (ImGui::IsItemClicked()) {
						GUIState::get_instance().object_selected = true;
						GUIState::get_instance().selected_object = NULL;
					}

					ImGui::TreePop();
				}

				ImGui::SeparatorText("Objects");

				for (engine::Object& o : scene.objects) {
					draw_object_node(o);
				}
				ImGui::TreePop();
			}
		}
		ImGui::End();
	}
}

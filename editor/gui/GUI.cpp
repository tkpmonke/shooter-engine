#define RGFW_IMGUI_IMPLEMENTATION
#define IMGUI_IMPL_OPENGL_LOADER_GLEW

#include <stdint.h>
#include "imgui/imgui.h"
#include "imgui/imgui_stdlib.h"
#include "imgui/imgui_impl_rgfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "GUI.hpp"
#include "Scene.hpp"
#include "rendering/Camera.hpp"
#include "loading/AssetCache.hpp"
#include "imguizmo/ImGuizmo.h"
#include <glm/gtc/type_ptr.hpp>

namespace editor::GUI {
	struct gui_state {
		long int selected_object = -2;
		ImGuizmo::OPERATION operation = ImGuizmo::OPERATION::TRANSLATE;
		ImGuizmo::MODE mode = ImGuizmo::MODE::LOCAL;
	} gui_state;

	void initilize(engine::windowing::Window* window) {
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

		ImGui::StyleColorsDark();
		
		ImGui_ImplRgfw_InitForOpenGL(window->rgfw_window, true);
		ImGui_ImplOpenGL3_Init();
	}

	void begin_frame() {
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplRgfw_NewFrame();
		ImGui::NewFrame();
		ImGuizmo::BeginFrame();

		ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport(), ImGuiDockNodeFlags_PassthruCentralNode);
	}

	void finish_frame() {
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

	void shutdown() {
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplRgfw_Shutdown();
		ImGui::DestroyContext();
	}

	void draw_object_node(engine::Object& o) {
		ImGuiTreeNodeFlags is_leaf = o.children.empty() ? ImGuiTreeNodeFlags_Leaf : 0;
		if (ImGui::TreeNodeEx(o.name.data(), ImGuiTreeNodeFlags_DefaultOpen | is_leaf)) {
			
			if (ImGui::IsItemClicked()) {
				gui_state.selected_object = o.id;
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
			if (ImGui::TreeNodeEx(scene.name, flag)) {

				if (ImGui::TreeNodeEx("Camera", flag | ImGuiTreeNodeFlags_Leaf)) {
					if (ImGui::IsItemClicked()) {
						gui_state.selected_object = -2;
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

	void draw_shader_selector() {	
		ImGui::SetNextWindowPos(ImGui::GetMainViewport()->GetCenter(), ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
		if (ImGui::BeginPopupModal("Set Shader", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
			ImGui::Text("Select the shader to be used");

			static engine::rendering::Shader* selected;
			if (ImGui::BeginListBox("Shaders")) {
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
				engine::Scene::get_instance().objects[gui_state.selected_object].shader = selected;
				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
		}
	}

	void draw_inspector() {
		if (ImGui::Begin("Inspector")) {
			engine::Scene& scene = engine::Scene::get_instance();

			if (gui_state.selected_object < -1) {
				/// Camera Selected
				ImGui::Text("Camera");

				ImGui::SeparatorText("Transform");

				ImGui::InputFloat3("Position", glm::value_ptr(scene.camera.position));
				ImGui::InputFloat3("Rotation", glm::value_ptr(scene.camera.rotation));
				
				ImGui::SeparatorText("Camera");
				
				ImGui::InputFloat("FOV", &scene.camera.fov);
				ImGui::InputFloat("Min", &scene.camera.min);
				ImGui::InputFloat("Max", &scene.camera.max);

				if (ImGui::RadioButton("Perspective", 
						scene.camera.projection_mode == engine::rendering::Camera::projection_mode_perspective)) {
					scene.camera.projection_mode = engine::rendering::Camera::projection_mode_perspective;
					ImGui::SameLine();
				} if (ImGui::RadioButton("Orthographic", 
						scene.camera.projection_mode == engine::rendering::Camera::projection_mode_orthographic)) {
					scene.camera.projection_mode = engine::rendering::Camera::projection_mode_orthographic;
				}
			} else if (gui_state.selected_object != -1) {
				/// Object Selected
				engine::Object* obj = &scene.objects[gui_state.selected_object];

				ImGui::InputText("Name", &obj->name);
				ImGui::Text("ID : %lu", obj->id);

				ImGui::SeparatorText("Transform");

				ImGui::InputFloat3("Position", glm::value_ptr(obj->position));
				ImGui::InputFloat3("Rotation", glm::value_ptr(obj->rotation));
				ImGui::InputFloat3("Scale", glm::value_ptr(obj->scale));

				if (ImGui::Button(((std::string)"Set Shader (current: " + obj->shader->name + ")").data())) {
					ImGui::OpenPopup("Set Shader");
				}
			}
		}

		draw_shader_selector();

		ImGui::End();
	}

	void draw_gizmos() {
		if (gui_state.selected_object <= -1) return;
		
		engine::Scene& scene = engine::Scene::get_instance();
		
		ImGuiIO& io = ImGui::GetIO();
		ImGuizmo::SetRect(0, 0, io.DisplaySize.x, io.DisplaySize.y);
		ImGuizmo::SetOrthographic(scene.camera.projection_mode == engine::rendering::Camera::projection_mode_orthographic);

		glm::mat4 model = glm::mat4(1.f);
      model = glm::translate(model, scene.objects[gui_state.selected_object].position);
      model *= glm::mat4_cast(glm::quat(glm::radians(scene.objects[gui_state.selected_object].rotation)));
      model = glm::scale(model, scene.objects[gui_state.selected_object].scale);

		if (ImGuizmo::Manipulate(
			glm::value_ptr(scene.camera.view),
			glm::value_ptr(scene.camera.projection),
			gui_state.operation,
			gui_state.mode,
			glm::value_ptr(model))
		) {
			
			ImGuizmo::DecomposeMatrixToComponents(
				glm::value_ptr(model),
				glm::value_ptr(scene.objects[gui_state.selected_object].position),
				glm::value_ptr(scene.objects[gui_state.selected_object].rotation),
				glm::value_ptr(scene.objects[gui_state.selected_object].scale)
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

//		ImGuizmo::DrawCubes(glm::value_ptr(scene.camera.view), glm::value_ptr(scene.camera.projection), 
	}

	void draw_all() {
		begin_frame();
		draw_hierarchy();
		draw_inspector();
		draw_gizmos();
		finish_frame();
	}
}

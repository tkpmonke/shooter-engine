#define RGFW_IMGUI_IMPLEMENTATION
#define IMGUI_IMPL_OPENGL_LOADER_GLEW
#include "imgui/imgui_impl_rgfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "GUI.hpp"
#include "Scene.hpp"
namespace editor::gui {

	void initilize(engine::windowing::Window* window) {
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

		/// this is a workaround for the shitty imgui backend
		/// for rgfw
#ifdef _WIN32
		io.MouseDrawCursor = true;
#endif

		ImGui::StyleColorsDark();
		
		RGFW_setMousePosCallback(editor::gui::raw_mouse_callback);
		ImGui_ImplRgfw_InitForOpenGL(window->rgfw_window, true);
		ImGui_ImplOpenGL3_Init();

		initilize_highlights();

		GUIState::get_instance().window = window;
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
		shutdown_highlights();
	}

	void render_children(engine::Object& o, engine::rendering::Camera* c) {
		for (engine::Object& d : o.children) {
			d.render(c);
			render_children(d, c);
		}
	}

	void draw_all() {
		for (engine::Object& o : engine::Scene::get_instance().objects) {
			o.render(&GUIState::get_instance().camera);
			render_children(o, &GUIState::get_instance().camera);
		}

		draw_highlights();

		begin_frame();
		draw_gizmos();
		draw_hierarchy();
		draw_inspector();
		finish_frame();
	}

	GUIState& GUIState::get_instance() {
		static GUIState state;
		return state;
	}
}

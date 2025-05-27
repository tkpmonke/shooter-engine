#include "rendering/RenderingDevice.hpp"
#include "rendering/opengl/RenderingDevice.hpp"
#include "rendering/Mesh.hpp"
#include "windowing/Window.hpp"
#include "Object.hpp"
#include "Scene.hpp"
#include "loading/AssetCache.hpp"
#include "loading/SceneLoader.hpp"

#include "gui/GUI.hpp"

using namespace engine;
using namespace engine::windowing;
using namespace engine::rendering;

int main(void) {
	windowing::Window window;
	window.create(1280, 720, (char*)"Editor");

	RenderingDevice::create_rendering_device();
	gl::GLRenderingDevice* renderer = (gl::GLRenderingDevice*)RenderingDevice::get_instance();
	renderer->initilize();

	Scene& scene = Scene::get_instance();
	scene.camera.position = {0, 0, 5};

	loading::AssetCache& cache = loading::AssetCache::get_instance();
	cache.set_mode(cache.load_mode_folder);
	
	cache.load_shader("assets/default.vs.glsl", "assets/default.fs.glsl", "core/default");
	cache.load_shader("assets/default.vs.glsl", "assets/different_default.fs.glsl", "core/different_default");
	cache.load_mesh("assets/sphere.glb");

	loading::load_scene("assets/main.yaml");
	
	//Object* object = scene.create_object();
	//object->initilize(&cache.meshes["assets/sphere.glb"], &cache.shaders["core/default"]);

	editor::gui::initilize(&window);

	while (window.status()) {
		renderer->begin_frame();
		editor::gui::process_editor_camera();
		editor::gui::draw_all();

		window.present();
	}

	scene.shutdown();
	editor::gui::shutdown();
	loading::AssetCache::shutdown();
	renderer->shutdown();
	RenderingDevice::shutdown_instance();
	window.shutdown();
}

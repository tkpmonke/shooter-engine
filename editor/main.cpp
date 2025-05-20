#include "rendering/RenderingDevice.hpp"
#include "rendering/opengl/RenderingDevice.hpp"
#include "rendering/Mesh.hpp"
#include "windowing/Window.hpp"
#include "Object.hpp"
#include "Scene.hpp"
#include "loading/AssetCache.hpp"

#include "gui/GUI.hpp"

using namespace engine;
using namespace engine::windowing;
using namespace engine::rendering;

int main(void) {
	Window window;
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
	
	Shader* shader = &cache.shaders["core/default"];
	Mesh* mesh = (Mesh*)engine::loading::AssetCache::get_instance().load_mesh("assets/sphere.glb");

	Object* object = scene.create_object();
	object->initlize(mesh, shader);

	editor::GUI::initilize(&window);

	while (window.status()) {
		renderer->begin_frame();
		scene.camera.process(&window);

		scene.objects[0].rotation.x = window.time*15;
		scene.objects[0].rotation.y = window.time*12;
		scene.objects[0].rotation.z = window.time*20;
		scene.render_objects();

		editor::GUI::draw_all();

		window.present();
	}

	scene.shutdown();
	editor::GUI::shutdown();
	renderer->shutdown_shader(shader);
	renderer->shutdown();
	RenderingDevice::shutdown_instance();
	window.shutdown();
}

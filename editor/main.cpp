#include "rendering/RenderingDevice.hpp"
#include "rendering/opengl/RenderingDevice.hpp"
#include "windowing/Window.hpp"
#include "Object.hpp"
#include "loading/MeshLoader.hpp"

using namespace engine;
using namespace engine::windowing;
using namespace engine::rendering;

const std::vector<Vertex> square_vertices = {
   {.position = {0.5f,  0.5f, 0.0f}},
   {.position = {0.5f,  -0.5f, 0.0f}},
   {.position = {-0.5f,  -0.5f, 0.0f}},
   {.position = {-0.5f,  0.5f, 0.0f}}
};
const std::vector<unsigned int> square_indices = {
   0, 1, 3,
   1, 2, 3
};
   
const Mesh square_mesh = {
   .vertices = square_vertices,
   .indices = square_indices,
};

const char* vertex_shader = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
	 "uniform mat4 u_model;"
	 "uniform mat4 u_view;"
	 "uniform mat4 u_projection;"
    "out vec3 pos;\n"
    "void main()\n"
    "{\n"
    "   pos = aPos+0.5f;\n"
    "   gl_Position = u_projection*u_view*u_model*vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";
const char* fragment_shader = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "in vec3 pos;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(pos,1)*vec4(1.0f, 0.5f, 0.85f, 1.0f);\n"
    "}\n\0";

	

int main(void) {
	Window window;
	window.create(1280, 720, (char*)"Editor");

	RenderingDevice::create_rendering_device();
	gl::GLRenderingDevice* renderer = (gl::GLRenderingDevice*)RenderingDevice::get_instance();
	renderer->initilize();

	Camera camera;
	camera.position = {0,0,5};

	Shader shader = {
		.vertex_shader = (char*)vertex_shader,
		.fragment_shader = (char*)fragment_shader,
		.internal = 0
	};
	renderer->compile_shader(&shader);

	Mesh* mesh = engine::loading::LoadMesh("./sphere.glb");

	Object object;
	object.initlize(mesh, &shader);

	while (window.status()) {
		renderer->begin_frame();

		camera.process(&window);
		object.render(&camera);
		
		window.present();
	}

	renderer->shutdown_shader(&shader);
	renderer->shutdown();
	RenderingDevice::shutdown_instance();
	window.shutdown();
}

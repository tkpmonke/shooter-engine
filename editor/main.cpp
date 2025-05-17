#include "rendering/opengl/RenderingDevice.hpp"
#include "windowing/Window.hpp"

using namespace engine;
using namespace engine::windowing;
using namespace engine::rendering;
using namespace engine::rendering::gl;

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
    "out vec3 pos;\n"
    "void main()\n"
    "{\n"
    "   pos = aPos+0.5f;\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
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
	window.create(320, 200, (char*)"Editor");


	GLRenderingDevice renderer;
	renderer.initilize();
	Shader shader = {
		.vertex_shader = (char*)vertex_shader,
		.fragment_shader = (char*)fragment_shader
	};
	renderer.compile_shader(&shader);

	RenderObject* object = renderer.create_object((Mesh*)&square_mesh, &shader);

	while (window.status()) {
		renderer.begin_frame();
		renderer.draw(object);
		window.present();
	}

	renderer.shutdown_object(object);
	renderer.shutdown_shader(&shader);
	renderer.shutdown();
	window.shutdown();
}

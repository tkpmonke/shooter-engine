#include "RenderingDevice.hpp"
#include "Logging.hpp"

#include <glm/gtc/type_ptr.hpp>

#ifdef DEBUG
#include <stdio.h>
void GLAPIENTRY gl_message_callback(
						GLenum source,
				 	GLenum type, GLuint id,
				 	GLenum severity, GLsizei length,
				 	const GLchar* message, const void* userParam ) {
	if (severity == GL_DEBUG_SEVERITY_NOTIFICATION)
		return;

	/// unused arguments
	(void)source;
	(void)id;
	(void)length;
	(void)userParam;

	fprintf	( 	stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
				( 	type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : "" ),
				type, severity, message );
}
#endif

namespace engine::rendering::gl {
	void GLRenderingDevice::initilize() {
		if (glewInit() > 0) {
			engine::logging::print_error("GLEW failed to init");
			exit(1);
		}
		glEnable(GL_DEPTH_TEST);
		//glEnable(GL_CULL_FACE);

#ifdef DEBUG
		glEnable(GL_DEBUG_OUTPUT);
		glDebugMessageCallback(gl_message_callback, 0);
#endif
	}

	void GLRenderingDevice::begin_frame() {
		glClearColor(0.2f, 0.3f, 0.7f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void GLRenderingDevice::draw(RenderObject* o) {
		GLRenderObject* object = (GLRenderObject*)o;

		glBindVertexArray(object->vao);
		glUseProgram(object->program);
		glDrawElements(GL_TRIANGLES, object->indice_count, GL_UNSIGNED_INT, 0);
	}

	RenderObject* GLRenderingDevice::create_object(Mesh* mesh, Shader* shader) {
		GLRenderObject* object = (GLRenderObject*)malloc(sizeof(GLRenderObject));
		object->program = (unsigned long int)shader->internal;

		glGenVertexArrays(1, &object->vao);
		glBindVertexArray(object->vao);
		glGenBuffers(1, &object->vbo);
		glGenBuffers(1, &object->ebo);

		glBindBuffer(GL_ARRAY_BUFFER, object->vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)*mesh->vertices.size(), mesh->vertices.data(), GL_STATIC_DRAW); 

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, object->ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh->indices.size()*sizeof(int), mesh->indices.data(), GL_STATIC_DRAW);

		/// vertex position
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
		glEnableVertexAttribArray(0);
		/// normals
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
		glEnableVertexAttribArray(1);
		/// texture uv
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));
		glEnableVertexAttribArray(2);

		object->indice_count = mesh->indices.size();

		return (RenderObject*)object;
	}

	void GLRenderingDevice::compile_shader(Shader* s) {
		unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
   	glShaderSource(vertexShader, 1, (const char**)&s->vertex_shader, NULL);
   	glCompileShader(vertexShader);

   	int success;
   	char infoLog[512];
   	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
   	if (!success)
   	{
   	   glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
   	   engine::logging::print_error("GL_COMPILE_STATUS > VERTEX");
   	   engine::logging::print(infoLog);
   	}

   	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
   	glShaderSource(fragmentShader, 1, (const char**)&s->fragment_shader, NULL);
   	glCompileShader(fragmentShader);

   	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
   	if (!success)
   	{
   	   glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
   	   engine::logging::print_error("GL_COMPILE_STATUS > FRAGMENT");
   	   engine::logging::print(infoLog);
   	}

   	unsigned int shaderProgram = glCreateProgram();
   	glAttachShader(shaderProgram, vertexShader);
   	glAttachShader(shaderProgram, fragmentShader);
   	glLinkProgram(shaderProgram);

   	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
   	if (!success) {
   	   glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
   	   engine::logging::print_error("GL_LINK_STATUS");
   	   engine::logging::print(infoLog);
   	}
   	glDeleteShader(vertexShader);
   	glDeleteShader(fragmentShader);

		s->internal = (void*)(unsigned long int)shaderProgram;
	}

	void GLRenderingDevice::set_shader_float(Shader* shader, const char* name, float v) {
		glUseProgram((unsigned long int)shader->internal);
		glUniform1f(glGetUniformLocation((unsigned long int)shader->internal, name), v);
	}

	void GLRenderingDevice::set_shader_int(Shader* shader, const char* name, int v) {
		glUseProgram((unsigned long int)shader->internal);
		glUniform1f(glGetUniformLocation((unsigned long int)shader->internal, name), v);
	}

	void GLRenderingDevice::set_shader_vec2(Shader* shader, const char* name, glm::vec2 v) {
		glUseProgram((unsigned long int)shader->internal);
		glUniform2f(glGetUniformLocation((unsigned long int)shader->internal, name), v.x, v.y);
	}

	void GLRenderingDevice::set_shader_vec3(Shader* shader, const char* name, glm::vec3 v) {
		glUseProgram((unsigned long int)shader->internal);
		glUniform3f(glGetUniformLocation((unsigned long int)shader->internal, name), v.x, v.y, v.z);
	}

	void GLRenderingDevice::set_shader_vec4(Shader* shader, const char* name, glm::vec4 v) {
		glUseProgram((unsigned long int)shader->internal);
		glUniform4f(glGetUniformLocation((unsigned long int)shader->internal, name), v.x, v.y, v.z, v.w);
	}

	void GLRenderingDevice::set_shader_mat4(Shader* shader, const char* name, glm::mat4 v) {
		glUseProgram((unsigned long int)shader->internal);
		glUniformMatrix4fv(glGetUniformLocation((unsigned long int)shader->internal, name), 1, GL_FALSE, glm::value_ptr(v));
	}


	/// unused by opengl atm
	void GLRenderingDevice::shutdown() {
	}

	void GLRenderingDevice::shutdown_object(RenderObject* o) {
		GLRenderObject* object = (GLRenderObject*)o;

		glDeleteVertexArrays(1, &object->vao);
		glDeleteBuffers(1, &object->vbo);
		glDeleteBuffers(1, &object->ebo);

		free(object);
	}

	void GLRenderingDevice::shutdown_shader(Shader* o) {
		glDeleteProgram((unsigned long int)o->internal);
	}
}

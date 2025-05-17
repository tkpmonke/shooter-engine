#include "Window.hpp"
#include <GLFW/glfw3.h>

namespace engine::windowing {
	void Window::create(unsigned int width, unsigned int height, char* title) {
		glfwInit();
		this->internal = glfwCreateWindow(width, height, title, NULL, NULL);
		glfwMakeContextCurrent((GLFWwindow*)this->internal);
	}

	bool Window::status() {
		glfwPollEvents();

		static int pw, ph;
		glfwGetWindowSize((GLFWwindow*)this->internal, &this->width, &this->height);
		if (pw != this->width || ph != this->height) {
			glViewport(0,0, this->width, this->height);
			pw = this->width;
			ph = this->height;
		}

		this->time = glfwGetTime();

		return !glfwWindowShouldClose((GLFWwindow*)this->internal);
	}

	void Window::present() {
		glfwSwapBuffers((GLFWwindow*)this->internal);
	}

	void Window::shutdown() {
		glfwDestroyWindow((GLFWwindow*)this->internal);
		glfwTerminate();
	}
}

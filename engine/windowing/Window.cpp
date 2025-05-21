#include "Window.hpp"
#include "rendering/Camera.hpp"
#include "Logging.hpp"

#include <assert.h>
#include <string>

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <Windows.h> // for GL/GL.h
#include <GL/GL.h> // for glViewport
#endif

namespace engine::windowing {
	void Window::create(unsigned int width, unsigned int height, char* title) {
		RGFW_init();
		this->rgfw_window = RGFW_createWindow(title, RGFW_RECT(0, 0, width, height), RGFW_windowCenter);

		RGFW_window_makeCurrent(this->rgfw_window);
	}

	bool Window::status() {
		RGFW_window_checkEvents(this->rgfw_window, RGFW_eventNoWait);

		this->width = this->rgfw_window->r.w;
		this->height = this->rgfw_window->r.h;

		static int pw = 0, ph = 0;
		if (pw != this->width || ph != this->height) {
			glViewport(0,0, this->width, this->height);
			pw = this->width;
			ph = this->height;
		}

		this->time = RGFW_getTime();

		return !RGFW_window_shouldClose(this->rgfw_window);
	}

	void Window::present() {
		RGFW_window_swapBuffers(this->rgfw_window);
	}

	void Window::shutdown() {
		RGFW_window_close(this->rgfw_window);
	}
}

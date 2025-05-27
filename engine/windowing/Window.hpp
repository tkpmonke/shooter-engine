#pragma once

#include "crossplatform/Export.hpp"

extern "C" {
#define RGFW_IMPORT
#include "rgfw/RGFW.h"
}

namespace engine::windowing {
	class Window {
	public:
		ENGINE_CROSSPLATFORM_EXPORT void create(unsigned int width, unsigned int height, char* title);
		ENGINE_CROSSPLATFORM_EXPORT bool status();
		ENGINE_CROSSPLATFORM_EXPORT void present();
		ENGINE_CROSSPLATFORM_EXPORT void shutdown();

		int width, height;
		double time;

		RGFW_window* rgfw_window;
	};
}




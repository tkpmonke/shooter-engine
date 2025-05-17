#pragma once

namespace engine::windowing {
	class Window {
	public:
		void create(unsigned int width, unsigned int height, char* title);
		bool status();
		void present();
		void shutdown();

		int width, height;

		void* internal;
	};
}

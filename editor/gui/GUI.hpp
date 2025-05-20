#pragma once
#include "windowing/Window.hpp"

namespace editor::GUI {
	void initilize(engine::windowing::Window*);

	void begin_frame();

	void finish_frame();

	void shutdown();

	void draw_hierarchy();
	void draw_inspector();
	void draw_gizmos();

	/// draws all of the windows
	void draw_all();
}

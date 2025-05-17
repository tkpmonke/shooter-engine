#include "RenderingDevice.hpp"
#include "rendering/opengl/RenderingDevice.hpp"
#include "Logging.hpp"

namespace engine::rendering {
	static RenderingDevice* instance;
	void RenderingDevice::create_rendering_device(graphics_api api) {
		switch (api) {
			case (graphics_api_vulkan):
				engine::logging::print_error("Vulkan is not implemented yet");
				exit(1);
				break;
			case (graphics_api_opengl):
			default:
				instance = new gl::GLRenderingDevice();
				break;
		}
	}

	RenderingDevice* RenderingDevice::get_instance() {
		return instance;
	}

	void RenderingDevice::shutdown_instance() {
		free(instance);
	}
}

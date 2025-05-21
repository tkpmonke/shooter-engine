#pragma once

#include "crossplatform/Export.hpp"

namespace engine::logging {
	ENGINE_CROSSPLATFORM_EXPORT void print(char*);
	ENGINE_CROSSPLATFORM_EXPORT void print(const char*);
	ENGINE_CROSSPLATFORM_EXPORT void print_warning(char*);
	ENGINE_CROSSPLATFORM_EXPORT void print_warning(const char*);
	ENGINE_CROSSPLATFORM_EXPORT void print_error(char*);
	ENGINE_CROSSPLATFORM_EXPORT void print_error(const char*);
	ENGINE_CROSSPLATFORM_EXPORT void print_debug(char*);
	ENGINE_CROSSPLATFORM_EXPORT void print_debug(const char*);
}

#include "Logging.hpp"
#include <iostream>

namespace engine::logging {
	
	void print(char* s) { 
		std::cout << s << "\n";
	}

	void print(const char* s) { 
		std::cout << s << "\n";
	}
	
	void print_warning(char* s) { 
		std::cout << s << "\n";
	}

	void print_warning(const char* s) { 
		std::cout << s << "\n";
	}
	
	void print_error(char* s) { 
		std::cerr << s << "\n";
	}
	
	void print_error(const char* s) { 
		std::cerr << s << "\n";
	}
	
	void print_debug(char* s) { 
#ifdef DEBUG
	std::cout << s << "\n";
#endif
	}
	
	void print_debug(const char* s) { 
#ifdef DEBUG
	std::cout << s << "\n";
#endif
	}
}

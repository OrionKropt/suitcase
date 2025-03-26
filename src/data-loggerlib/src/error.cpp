#include "error.h"

#include <iostream>

auto print_error(const char* file, unsigned int line, const char* function,
	             const char* what, bool exit, const char* extra_print) -> void
{
	std::cerr << "ERROR: \'" << what << '\'' << std::endl;
	std::cerr << "\tIn " << function << std::endl;
	std::cerr << '\t' << file << ", line " << line << std::endl;
	if (extra_print != nullptr)
	{
		std::cerr << extra_print << std::endl;
	}
	if (exit)
	{
		std::exit(EXIT_FAILURE);
	}
}


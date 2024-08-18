#include "error.h"
#include <iostream>


auto print_error(const char* file, unsigned int line, const char* function,
	const char* what, bool exit, const char* extra_print) -> void
{

	std::cerr << "ERROR: \'" << what << "\'\n";
	std::cerr << "\tIn " << function << '\n';
	std::cerr << '\t' << file << ", line " << line << '\n';
	if (extra_print != nullptr)
	{
		std::cerr << extra_print << '\n';
	}
	if (exit)
	{
		std::exit(EXIT_FAILURE);
	}
}


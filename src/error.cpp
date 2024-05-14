#include "error.h"

auto print_error(const char* class_name, const char* method_name, const char* what, bool exit, unsigned int line, const char* file, std::function<void()> extra_print) -> void
{
	std::cerr << "ERROR::" << class_name << "::" << method_name << ": " << what << '\n';
	std::cerr << "File: " << file << '\n';
	std::cerr << "Line: " << line << '\n';
	if (extra_print != nullptr)
	{
	extra_print();
	}
	if (exit)
	{
		std::exit(EXIT_FAILURE);
	}
}

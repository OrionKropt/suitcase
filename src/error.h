#pragma once

#include <format>

                        // Becouse it's inpossible to put const char str whit macro
#define EMPTY_STR ""    //------------------------------------------------------- |
																			//    |
auto print_error(const char* file, unsigned int line, const char* function, //    |
	const char* what, bool exit, const char* extra_print) -> void;          //    |
                                                                            //   \ /
#define PRINT_ERROR(what, exit, ...) \
	{print_error(__FILE__, __LINE__, __FUNCTION__, (what), (exit), std::format(EMPTY_STR __VA_ARGS__).c_str());}

#pragma once

#include <iostream>

auto print_error(const char* class_name, const char* method_name, const char* what, bool exit, unsigned int line, const char* file, void (*extra_print)() = nullptr) -> void;
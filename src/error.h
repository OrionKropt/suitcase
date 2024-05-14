#pragma once

#include <iostream>
#include <functional>

auto print_error(const char* class_name, const char* method_name, const char* what, bool exit, unsigned int line, const char* file, std::function<void()> extra_print = nullptr) -> void;
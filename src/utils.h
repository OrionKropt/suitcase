#pragma once

#include <string>
#include <glad.h>

auto to_wstring(const std::string& str) -> std::wstring;
auto to_wchar_t(const char* str) -> const wchar_t*;

// Converts given time in seconds/minutes/hours to milliseconds cause Graph steps use ms
auto operator""_s(GLuint64 time) -> GLfloat;
auto operator""_s(long double time) -> GLfloat;

auto operator""_m(GLuint64 time) -> GLfloat;
auto operator""_m(long double time) -> GLfloat;

auto operator""_h(GLuint64 time) -> GLfloat;
auto operator""_h(long double time) -> GLfloat;

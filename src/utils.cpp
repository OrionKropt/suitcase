#include "utils.h"

#include <cwchar>
#include <locale>
#include <codecvt>

auto to_wstring(const std::string& str) -> std::wstring
{
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    return std::move(converter.from_bytes(str));
}

auto to_wchar_t(const char* str) -> const wchar_t*
{
    size_t length = std::mbstowcs(nullptr, str, 0) + 1;
    wchar_t* res = new wchar_t[length];
    std::mbstowcs(res, str, length);
    return res;
}

auto operator""_s(GLuint64 time) -> GLfloat
{
    return (GLfloat) time * 1000;
}
auto operator""_s(long double time) -> GLfloat
{
    return (GLfloat) time * 1000;
}

auto operator""_m(GLuint64 time) -> GLfloat
{
    return (GLfloat) time * 60000;
}
auto operator""_m(long double time) -> GLfloat
{
    return (GLfloat) time * 60000;
}

auto operator""_h(GLuint64 time) -> GLfloat
{
    return (GLfloat) time * 3600000;
}
auto operator""_h(long double time) -> GLfloat
{
    return (GLfloat) time * 3600000;
}

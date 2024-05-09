#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class OpenGL {
public:
    // ������� ���������. ��� ������ ������ ������ ������ ������.
    // ��� ���� ����������� - ���������� ������ �� ������������ ������������.
    static auto get_instance() -> OpenGL&;

    auto initialize() -> void;

    [[nodiscard]]
    auto create_window(int width, int height, const char* title) -> GLFWwindow*;
    auto destroy_window(GLFWwindow* window) -> void;
    [[nodiscard]]
    auto create_shader_program(const char*&& vertex_file, const char*&& fragment_file) -> GLuint;

private:
    OpenGL();
    ~OpenGL();

    // ������� ��������� ������ ��� ��������� ������� ����
    static auto framebuffer_resize_callback(GLFWwindow* window, int width, int height) -> void;
};

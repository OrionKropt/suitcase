#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class OpenGL {
public:
    // Паттерн синглтона. При первом вызове создаёт объект класса.
    // При всех последующих - возвращает ссылку на единственный существующий.
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

    // Функция обратного вызова для изменения размера окна
    static auto framebuffer_resize_callback(GLFWwindow* window, int width, int height) -> void;
};

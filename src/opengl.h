#pragma once

#include <memory>
#include <string>
#include <vector>
#include <unordered_map>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

class Shader;

class OpenGL
{
    struct Character;
public:
    static auto get_instance() -> OpenGL&;

    auto initialize() -> void;

    auto get_window() -> GLFWwindow*;
    auto create_shader(const char* name, const char* vertex_file, const char* fragment_file, const char* geometry_file = nullptr) -> std::shared_ptr<Shader>;
    auto get_shader(const char* name) -> std::shared_ptr<Shader>;
    auto get_char(char c) -> const Character*;
    auto get_key(int key_code) -> GLboolean;
    auto get_window_width() -> GLint;
    auto get_window_height() -> GLint;

private:
    struct Character
    {
        GLuint      texture;
        glm::ivec2  size;
        glm::ivec2  bearing;
        GLuint      advance;
    };

    std::unordered_map<char, Character>                         characters;
    std::unordered_map<std::string, std::shared_ptr<Shader>>    shaders;
    GLFWwindow*                                                 window;

    OpenGL();
    ~OpenGL();

    static GLint                    window_width;
    static GLint                    window_height;
    static std::vector<GLboolean>   keys;

    static auto key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) -> void;
    static auto framebuffer_resize_callback(GLFWwindow* window, int width, int height) -> void;
};


class Shader
{
public:
    Shader(const char* vertex_path, const char* fragment_path, const char* geometry_path = nullptr);

    auto use() -> void;
    auto get_shader_program() -> GLuint;
    
    auto set_bool(const char* name, GLboolean value) -> void;
    auto set_int(const char* name, GLint value) -> void;
    auto set_float(const char* name, GLfloat value) -> void;
    auto set_vec2(const char* name, glm::vec2& value) -> void;
    auto set_vec2(const char* name, GLfloat x, GLfloat y) -> void;
    auto set_vec3(const char* name, glm::vec3& value) -> void;
    auto set_vec3(const char* name, GLfloat x, GLfloat y, GLfloat z) -> void;
    auto set_vec4(const char* name, glm::vec4& value) -> void;
    auto set_vec4(const char* name, GLfloat x, GLfloat y, GLfloat z, GLfloat w) -> void;
    auto set_mat2(const char* name, glm::mat2& value) -> void;
    auto set_mat3(const char* name, glm::mat3& value) -> void;
    auto set_mat4(const char* name, glm::mat4& value) -> void;

private:
    auto check_compile_errors(GLuint target, const std::string&& type) -> void;

    GLuint shader_program;
};

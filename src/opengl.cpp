#include "opengl.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>
#include <ft2build.h>
#include FT_FREETYPE_H
#include "error.h"

std::vector<GLboolean> OpenGL::keys(1024, false);
std::vector<GLboolean> OpenGL::mouse_buttons(8, false);
GLint OpenGL::window_width;
GLint OpenGL::window_height;
GLfloat OpenGL::mouse_x;
GLfloat OpenGL::mouse_y;

OpenGL::OpenGL() = default;

OpenGL::~OpenGL() = default;

auto OpenGL::get_instance() -> OpenGL&
{
    static OpenGL instance;
    return instance;
}

auto OpenGL::initialize() -> void
{
    window_width = 800;
    window_height = 800;


    // *                                    GLFW initialization
    // * -------------------------------------------------------------------------------------------
    if (!glfwInit())
    {
        PRINT_ERROR("GLFW initialization failed", true);
    }

    glfwSetErrorCallback(error_callback);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, 4);        // 4x MSAA

    glfwWindowHint(GLFW_FOCUSED, GLFW_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    window = glfwCreateWindow(window_width, window_height, "PM710 Graphics", nullptr, nullptr);
    if (!window)
    {
        glfwTerminate();
        PRINT_ERROR("Window creation failed", true);
    }

    // Callbacks
    glfwSetKeyCallback(window, key_callback);
    glfwSetFramebufferSizeCallback(window, framebuffer_resize_callback);
    glfwSetCursorPosCallback(window, cursor_position_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);    // Enable VSync


    // *                                    GLAD initialization
    // * -------------------------------------------------------------------------------------------
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
    {
        glfwTerminate();
        PRINT_ERROR("GLAD initialization failed", true);
    }

    glViewport(0, 0, window_width, window_height);
    glEnable(GL_MULTISAMPLE);   // MSAA for smooth lines


    // *                                  FreeType initialization
    // * -------------------------------------------------------------------------------------------
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // ? GL_CULL_FACE disables Point render. Why?
    // glEnable(GL_CULL_FACE);

    FT_Library ft;
    if (FT_Init_FreeType(&ft))
    {
        glfwTerminate();
        PRINT_ERROR("FreeType initialization failed", true);
    }

    FT_Face face;
    const char* font_path = "resources/fonts/arial.ttf";
    if (FT_New_Face(ft, font_path, 0, &face))
    {
        glfwTerminate();
        PRINT_ERROR("Font load failed", true, "Path to font: '{}'\n", font_path);
    }

    FT_Set_Pixel_Sizes(face, 0, 48);
    FT_Select_Charmap(face, ft_encoding_unicode);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    // Unicode blocks:
    // [0x0000, 0x007F] - Basic Latin (ASCII), [0x0400, 0x04FF] - Cyrillic
    for (wchar_t c = 0x0000; c <= 0x04FF; ++c)
    {
        // When done with ASCII, switch to cyrillic
        if (c > 0x007F && c < 0x0400)
        {
            c = 0x0400;
        }

        FT_UInt glyph_index = FT_Get_Char_Index(face, c);

        // glyph_index == 0 means that current font doesn't support given character
        if (!glyph_index)
        {
            continue;
        }

        if (FT_Load_Glyph(face, glyph_index, FT_LOAD_RENDER))
        {
            PRINT_ERROR("Glyph load failed", false, "Glyph: '{}'\n", (unsigned long) c);
            continue;
        }

        GLuint texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(
                GL_TEXTURE_2D,
                0,
                GL_RED,
                (GLsizei) face->glyph->bitmap.width,
                (GLsizei) face->glyph->bitmap.rows,
                0,
                GL_RED,
                GL_UNSIGNED_BYTE,
                face->glyph->bitmap.buffer
        );

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        auto character = std::make_shared<Character>(
                texture,
                glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
                glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
                (GLuint) (face->glyph->advance.x)
                );
        characters.insert(std::make_pair(c, character));
    }

    glBindTexture(GL_TEXTURE_2D, 0);

    FT_Done_Face(face);
    FT_Done_FreeType(ft);
}

auto OpenGL::get_window() -> GLFWwindow*
{
    return window;
}

auto OpenGL::create_shader(const char* name, const char* vertex_file, const char* fragment_file,
                           const char* geometry_file) -> std::shared_ptr<Shader>
{
    auto shader = std::make_shared<Shader>(vertex_file, fragment_file, geometry_file);
    shaders[name] = shader;
    return shader;
}

auto OpenGL::get_shader(const char* name) -> std::shared_ptr<Shader>
{
    return shaders[name];
}

auto OpenGL::get_char(wchar_t c) -> std::shared_ptr<Character>
{
    return characters[c];
}

auto OpenGL::get_key(int key_code) -> GLboolean
{
    return keys[key_code];
}

auto OpenGL::get_mouse_button(int button) -> GLboolean
{
    return mouse_buttons[button];
}

auto OpenGL::get_window_width() -> GLint
{
    return window_width;
}

auto OpenGL::get_window_height() -> GLint
{
    return window_height;
}

auto OpenGL::get_mouse_x() -> GLfloat
{
    return mouse_x;
}

auto OpenGL::get_mouse_y() -> GLfloat
{
    return mouse_y;
}

auto OpenGL::error_callback(int error, const char* what) -> void
{
    PRINT_ERROR("OpenGL error occurred.", false, "Details: {}\nCode: {}\n", what, error);
}

auto OpenGL::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) -> void
{
    keys[key] = action;
}

auto OpenGL::framebuffer_resize_callback(GLFWwindow* window, int width, int height) -> void
{
    glViewport(0, 0, width, height);
    window_width = width;
    window_height = height;
}

auto OpenGL::cursor_position_callback(GLFWwindow* window, double x, double y) -> void
{
    mouse_x = 2 * (x / window_width) - 1;
    mouse_y = 1 - 2 * (y / window_height);
}

auto OpenGL::mouse_button_callback(GLFWwindow* window, int button, int action, int mods) -> void
{
    mouse_buttons[button] = action;
}

Shader::Shader(const char* vertex_path, const char* fragment_path, const char* geometry_path)
{
    std::string vertex_code_raw;
    std::string fragment_code_raw;
    std::string geometry_code_raw;

    std::ifstream vertex_shader_file_stream;
    std::ifstream fragment_shader_file_stream;
    std::ifstream geometry_shader_file_stream;

    vertex_shader_file_stream.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fragment_shader_file_stream.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    geometry_shader_file_stream.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try
    {
        vertex_shader_file_stream.open(vertex_path);
        fragment_shader_file_stream.open(fragment_path);
        std::stringstream vertex_shader_string_stream;
        std::stringstream fragment_shader_string_stream;
        vertex_shader_string_stream << vertex_shader_file_stream.rdbuf();
        fragment_shader_string_stream << fragment_shader_file_stream.rdbuf();
        vertex_shader_file_stream.close();
        fragment_shader_file_stream.close();

        vertex_code_raw = vertex_shader_string_stream.str();
        fragment_code_raw = fragment_shader_string_stream.str();

        if (geometry_path != nullptr)
        {
            geometry_shader_file_stream.open(geometry_path);
            std::stringstream geometry_shader_string_stream;
            geometry_shader_string_stream << geometry_shader_file_stream.rdbuf();
            geometry_shader_file_stream.close();

            geometry_code_raw = geometry_shader_string_stream.str();
        }
    }
    catch (std::ifstream::failure& exception)
    {
        PRINT_ERROR("Shader file read failed", true, "Details: {}\n", exception.what());
    }

    const GLchar* vertex_code = vertex_code_raw.c_str();
    const GLchar* fragment_code = fragment_code_raw.c_str();

    GLuint vertex;
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vertex_code, nullptr);
    glCompileShader(vertex);
    check_compile_errors(vertex, "VERTEX");

    GLuint fragment;
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fragment_code, nullptr);
    glCompileShader(fragment);
    check_compile_errors(fragment, "FRAGMENT");

    GLuint geometry;
    if (geometry_path != nullptr)
    {
        const GLchar* geometry_code = geometry_code_raw.c_str();
        geometry = glCreateShader(GL_GEOMETRY_SHADER);
        glShaderSource(geometry, 1, &geometry_code, nullptr);
        glCompileShader(geometry);
        check_compile_errors(geometry, "GEOMETRY");
    }

    shader_program = glCreateProgram();

    glAttachShader(shader_program, vertex);
    glAttachShader(shader_program, fragment);
    if (geometry_path != nullptr)
    {
        glAttachShader(shader_program, geometry);
    }

    glLinkProgram(shader_program);
    check_compile_errors(shader_program, "PROGRAM");

    glDeleteShader(vertex);
    glDeleteShader(fragment);
    if (geometry_path != nullptr)
    {
        glDeleteShader(geometry);
    }
}

auto Shader::use() -> void
{
    glUseProgram(shader_program);
}

auto Shader::set_bool(const char* name, GLboolean value) -> void
{
    glUniform1i(glGetUniformLocation(shader_program, name), value);
}

auto Shader::set_int(const char* name, GLint value) -> void
{
    glUniform1i(glGetUniformLocation(shader_program, name), value);
}

auto Shader::set_float(const char* name, GLfloat value) -> void
{
    glUniform1f(glGetUniformLocation(shader_program, name), value);
}

auto Shader::set_vec2(const char* name, glm::vec2& value) -> void
{
    glUniform2fv(glGetUniformLocation(shader_program, name), 1, glm::value_ptr(value));
}

auto Shader::set_vec2(const char* name, GLfloat x, GLfloat y) -> void
{
    glUniform2f(glGetUniformLocation(shader_program, name), x, y);
}

auto Shader::set_vec3(const char* name, glm::vec3& value) -> void
{
    glUniform3fv(glGetUniformLocation(shader_program, name), 1, glm::value_ptr(value));
}

auto Shader::set_vec3(const char* name, GLfloat x, GLfloat y, GLfloat z) -> void
{
    glUniform3f(glGetUniformLocation(shader_program, name), x, y, z);
}

auto Shader::set_vec4(const char* name, glm::vec4& value) -> void
{
    glUniform4fv(glGetUniformLocation(shader_program, name), 1, glm::value_ptr(value));
}

auto Shader::set_vec4(const char* name, GLfloat x, GLfloat y, GLfloat z, GLfloat w) -> void
{
    glUniform4f(glGetUniformLocation(shader_program, name), x, y, z, w);
}

auto Shader::set_mat2(const char* name, glm::mat2& value) -> void
{
    glUniformMatrix2fv(glGetUniformLocation(shader_program, name), 1, GL_FALSE, glm::value_ptr(value));
}

auto Shader::set_mat3(const char* name, glm::mat3& value) -> void
{
    glUniformMatrix3fv(glGetUniformLocation(shader_program, name), 1, GL_FALSE, glm::value_ptr(value));
}

auto Shader::set_mat4(const char* name, glm::mat4& value) -> void
{
    glUniformMatrix4fv(glGetUniformLocation(shader_program, name), 1, GL_FALSE, glm::value_ptr(value));
}

auto Shader::get_shader_program() -> GLuint
{
    return shader_program;
}

auto Shader::check_compile_errors(GLuint target, const std::string&& type) -> void
{
    GLint success;
    GLchar log[1024];

    if (type == "VERTEX" || type == "FRAGMENT" || type == "GEOMETRY")
    {
        glGetShaderiv(target, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(target, 1024, nullptr, log);
            PRINT_ERROR("Shader compilation failed", true, "Shader: {} \nLog: {}\n", type, log);
        }
    }
    else if (type == "PROGRAM")
    {
        glGetProgramiv(target, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(target, 1024, nullptr, log);
            PRINT_ERROR("Shader program linking failed", true, "Log: {}\n", log);
        }
    }
    else
    {
        PRINT_ERROR("Unknown type specified", false, "Type: {}\n", type);
    }
}

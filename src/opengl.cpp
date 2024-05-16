#include "opengl.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>
#include <ft2build.h>
#include FT_FREETYPE_H

GLint OpenGL::window_width;
GLint OpenGL::window_height;

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


    //                                    GLFW initialization
    // -------------------------------------------------------------------------------------------
    if (!glfwInit())
    {
        std::cerr << "ERROR::OPENGL::INITIALIZE::GLFW_INIT_FAILED" << std::endl;
        exit(EXIT_FAILURE);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    glfwWindowHint(GLFW_FOCUSED, GLFW_TRUE);

    window = glfwCreateWindow(window_width, window_height, "PM710 Graphics", nullptr, nullptr);
    if (!window)
    {
        std::cerr << "ERROR::OPENGL::INITIALIZE::TEMP_WINDOW_CREATION_FAILED" << std::endl;
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(window);


    //                                    GLAD initialization
    // -------------------------------------------------------------------------------------------
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
    {
        std::cerr << "ERROR::OPENGL::INITIALIZE::GLAD_INIT_FAILED" << std::endl;
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glViewport(0, 0, window_width, window_height);

    //                                  FreeType initialization
    // -------------------------------------------------------------------------------------------
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // ? GL_CULL_FACE disables Point render. Why?
    // glEnable(GL_CULL_FACE);

    FT_Library ft;
    if (FT_Init_FreeType(&ft))
    {
        std::cerr << "ERROR::FREETYPE::INITIALIZE::LIB_INIT_FAILED" << std::endl;
        exit(EXIT_FAILURE);
    }

    // TODO: Replace hardcoded path to font. Need to refactor CMakeLists for that.
    FT_Face face;
    if (FT_New_Face(ft, "../src/fonts/arial.ttf", 0, &face))
    {
        std::cerr << "ERROR::FREETYPE::INITIALIZE::FONT_LOAD_FAILED" << std::endl;
        exit(EXIT_FAILURE);
    }

    FT_Set_Pixel_Sizes(face, 0, 48);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    std::string alphabet = " !\"#$%&\\'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[]^_`abcdefghijklmnopqrstuvwxyz{|}~јЅ¬√ƒ≈®∆«»… ЋћЌќѕ–—“”‘’÷„ЎўЏџ№ЁёяабвгдеЄжзийклмнопрстуфхцчшщъыьэю€";

    for (auto& c : alphabet) {
        if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
            std::cerr << "ERROR::FREETYPE::INITIALIZE::GLYPH_LOAD_FAILED" << std::endl;
            std::cerr << "Glyph: " << c << std::endl;
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

        Character character = {
                texture,
                glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
                glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
                (GLuint) (face->glyph->advance.x)
        };
        characters.insert(std::make_pair(c, character));
    }

    glBindTexture(GL_TEXTURE_2D, 0);

    FT_Done_Face(face);
    FT_Done_FreeType(ft);

    glGenVertexArrays(1, &text_VAO);
    glGenBuffers(1, &text_VBO);
    glBindVertexArray(text_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, text_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, nullptr, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

// ! Candidate for deletion cause we'll need only one window
auto OpenGL::create_window(int width, int height, const char* title) -> GLFWwindow*
{
    GLFWwindow* window;
    window = glfwCreateWindow(width, height, title, nullptr, nullptr);

    // ≈сли не удалось создать окно
    if (!window)
    {
        std::cerr << "ERROR::OPENGL::CREATE_WINDOW::FAILED" << std::endl;
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    // ”становка текущего контекста OpenGL
    glfwMakeContextCurrent(window);

    // Ќастройка размеров вьюпорта
    glViewport(0, 0, width, height);

    window_width = width;
    window_height = height;

    glfwSetFramebufferSizeCallback(window, framebuffer_resize_callback);

    return window;
}

// ! Candidate for deletion cause we'll need only one window.
// ! It's functionality will be replaced with OpenGL::shutdown()
auto OpenGL::destroy_window(GLFWwindow* window) -> void
{
    glfwDestroyWindow(window);
}

auto OpenGL::get_window() -> GLFWwindow*
{
    return window;
}

auto OpenGL::create_shader(const char* name, const char* vertex_file, const char* fragment_file, const char* geometry_file) -> Shader*
{
    auto* shader = new Shader(vertex_file, fragment_file, geometry_file);
    shaders[name] = shader;
    return shader;
}

auto OpenGL::get_shader(const char* name) -> Shader*
{
    return shaders[name];
}

auto OpenGL::render_text(Shader& shader, const std::string& text, float x, float y, float scale, glm::vec3 color) -> void
{
    shader.use();
    shader.set_vec3("text_color", color);

    glm::mat4 projection = glm::ortho(0.0f, (GLfloat) window_width, 0.0f, (GLfloat) window_height);
    shader.set_mat4("projection", projection);

    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(text_VAO);

    for (auto& c : text)
    {
        Character ch = characters[c];

        float x_pos = x + (GLfloat) ch.bearing.x * scale;
        float y_pos = y - (GLfloat) (ch.size.y - ch.bearing.y) * scale;

        float width  = (GLfloat) ch.size.x * scale;
        float height = (GLfloat) ch.size.y * scale;

        float vertices[] = {
                x_pos,          y_pos + height,     0.0f, 0.0f,
                x_pos,          y_pos,              0.0f, 1.0f,
                x_pos + width,  y_pos,              1.0f, 1.0f,

                x_pos,          y_pos + height,     0.0f, 0.0f,
                x_pos + width,  y_pos,              1.0f, 1.0f,
                x_pos + width,  y_pos + height,     1.0f, 0.0f
        };

        glBindTexture(GL_TEXTURE_2D, ch.texture);
        glBindBuffer(GL_ARRAY_BUFFER, text_VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glDrawArrays(GL_TRIANGLES, 0, 6);

        x += (GLfloat) (ch.advance >> 6) * scale;
    }
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

auto OpenGL::get_window_width() -> GLint
{
    return window_width;
}

auto OpenGL::get_window_height() -> GLint
{
    return window_height;
}

auto OpenGL::framebuffer_resize_callback(GLFWwindow* window, int width, int height) -> void
{
    glViewport(0, 0, width, height);
    window_width = width;
    window_height = height;
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
        std::cerr << "ERROR::SHADER::CONSTRUCTOR::FILE_READ_ERROR" << std::endl;
        std::cerr << "Error: " << exception.what() << std::endl;
        std::cerr << "Code: " << exception.code() << std::endl;
        exit(EXIT_FAILURE);
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
            std::cerr << "ERROR::SHADER::CONSTRUCTOR::COMPILATION_FAILED" << std::endl;
            std::cerr << "Shader: " << type << std::endl;
            exit(EXIT_FAILURE);
        }
    }
    else if (type == "PROGRAM")
    {
        glGetProgramiv(target, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(target, 1024, nullptr, log);
            std::cerr << "ERROR::SHADER::CONSTRUCTOR::PROGRAM_LINK_FAILED" << std::endl;
            exit(EXIT_FAILURE);
        }
    }
    else
    {
        std::cerr << "WARNING::SHADER::CONSTRUCTOR::UNKNOWN_TARGET_TYPE" << std::endl;
    }
}

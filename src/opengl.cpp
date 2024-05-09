#include "opengl.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <sstream>

OpenGL::OpenGL() = default;

OpenGL::~OpenGL() = default;

auto OpenGL::get_instance() -> OpenGL&
{
    static OpenGL instance;
    return instance;
}

auto OpenGL::initialize() -> void
{
    // Инициализация GLFW
    if (!glfwInit())
    {
        std::cerr << "ERROR::OPENGL::INITIALIZE::GLFW_INIT_FAILED" << std::endl;
        exit(EXIT_FAILURE);
    }

    // Версия OpenGL и профиль
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    glfwWindowHint(GLFW_FOCUSED, GLFW_TRUE);

    // Создание временного окна GLFW
    GLFWwindow* temp_window = glfwCreateWindow(1, 1, "", nullptr, nullptr);
    if (!temp_window)
    {
        std::cerr << "ERROR::OPENGL::INITIALIZE::TEMP_WINDOW_CREATION_FAILED" << std::endl;
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    // Устанавливаем текущий контекст OpenGL для временного окна
    glfwMakeContextCurrent(temp_window);

    // Загрузка указателей на OpenGL-функции с помощью GLAD
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
    {
        std::cerr << "ERROR::OPENGL::INITIALIZE::GLAD_INIT_FAILED" << std::endl;
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwDestroyWindow(temp_window);
}

auto OpenGL::create_window(int width, int height, const char* title) -> GLFWwindow*
{
    GLFWwindow* window;
    window = glfwCreateWindow(width, height, title, nullptr, nullptr);

    // Если не удалось создать окно
    if (!window)
    {
        std::cerr << "ERROR::OPENGL::CREATE_WINDOW::FAILED" << std::endl;
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    // Установка текущего контекста OpenGL
    glfwMakeContextCurrent(window);

    // Настройка размеров вьюпорта
    glViewport(0, 0, width, height);

    glfwSetFramebufferSizeCallback(window, framebuffer_resize_callback);

    return window;
}

auto OpenGL::destroy_window(GLFWwindow *window) -> void
{
    glfwDestroyWindow(window);
}

auto OpenGL::create_shader_program(const char*&& vertex_file, const char*&& fragment_file) -> GLuint
{
    std::string vertex_code_raw;
    std::string fragment_code_raw;
    std::ifstream vertex_shader_file_stream;
    std::ifstream fragment_shader_file_stream;

    vertex_shader_file_stream.exceptions(std::ifstream::failbit);
    fragment_shader_file_stream.exceptions(std::ifstream::failbit);

    try
    {
        vertex_shader_file_stream.open(vertex_file);
        fragment_shader_file_stream.open(fragment_file);

        std::stringstream vertex_shader_string_stream;
        std::stringstream fragment_shader_string_stream;
        vertex_shader_string_stream << vertex_shader_file_stream.rdbuf();
        fragment_shader_string_stream << fragment_shader_file_stream.rdbuf();
        vertex_shader_file_stream.close();
        fragment_shader_file_stream.close();

        vertex_code_raw = vertex_shader_string_stream.str();
        fragment_code_raw = fragment_shader_string_stream.str();
    }
    catch (std::ifstream::failure& e)
    {
        std::cerr << "ERROR::OPENGL::SHADER::FILE_NOT_SUCCESSFULLY_READ" << std::endl;
        return 0;
    }

    const GLchar* vertexCode = vertex_code_raw.c_str();
    const GLchar* fragmentCode = fragment_code_raw.c_str();
    GLuint vertex, fragment;
    GLint success;
    GLchar infoLog[512];

    // Vertex
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vertexCode, nullptr);
    glCompileShader(vertex);
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertex, 512, nullptr, infoLog);
        std::cerr << "ERROR::OPENGL::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
        return 0;
    }

    // Fragment
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fragmentCode, nullptr);
    glCompileShader(fragment);
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragment, 512, nullptr, infoLog);
        std::cerr << "ERROR::OPENGL::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
        return 0;
    }

    // Program
    GLuint programID;
    programID = glCreateProgram();
    glAttachShader(programID, vertex);
    glAttachShader(programID, fragment);
    glLinkProgram(programID);
    glGetProgramiv(programID, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(programID, 512, nullptr, infoLog);
        std::cerr << "ERROR::OPENGL::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
        return 0;
    }

    glDeleteShader(vertex);
    glDeleteShader(fragment);

    return programID;
}

auto OpenGL::framebuffer_resize_callback(GLFWwindow* window, int width, int height) -> void
{
    glViewport(0, 0, width, height);
}

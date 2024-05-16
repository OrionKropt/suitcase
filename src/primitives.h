#pragma once

#include <string>
#include <unordered_map>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "opengl.h"

class Primitive
{
public:
    virtual ~Primitive() = 0;
    virtual auto draw() -> void = 0;
};


class Point : public Primitive
{
public:
    explicit Point(glm::vec2 point, GLfloat width = 0.015f, glm::vec3 color = glm::vec3(1.0f, 0.0f, 0.0f));
    Point(GLfloat point_x, GLfloat point_y, GLfloat width = 0.015f, GLfloat R = 1.0, GLfloat G = 0.0f, GLfloat B = 0.0f);
    ~Point() override;

    auto draw() -> void override;
    auto set_color(glm::vec3 new_color) -> void;
    auto set_color(GLfloat R, GLfloat G, GLfloat B) -> void;
    auto set_width(GLfloat new_width) -> void;
    auto set_shader(Shader* new_shader) -> void;
    auto set_shader(const char* shader_name) -> void;

private:
    Shader*     shader;
    GLuint      VAO;
    GLuint      VBO;
    GLuint      EBO;
    glm::vec2   point;
    glm::vec3   color;          // [0..1] RGB
    GLfloat     width;
};


class Line : public Primitive
{
public:
    Line(glm::vec2 start, glm::vec2 end, GLfloat width = 7.0f, glm::vec3 color = glm::vec3(1.0f, 0.0f, 0.0f));
    Line(GLfloat start_x, GLfloat start_y, GLfloat end_x, GLfloat end_y, GLfloat width = 7.0f, GLfloat R = 1.0f, GLfloat G = 0.0f, GLfloat B = 0.0f);
    ~Line() override;

    auto draw() -> void override;
    auto set_color(glm::vec3 new_color) -> void;
    auto set_color(GLfloat R, GLfloat G, GLfloat B) -> void;
    auto set_width(GLfloat new_width) -> void;
    auto set_shader(Shader* new_shader) -> void;
    auto set_shader(const char* shader_name) -> void;

private:
    Shader*     shader;
    GLuint      VAO;
    GLuint      VBO;
    glm::vec2   start;
    glm::vec2   end;
    glm::vec3   color;          // [0..1] RGB
    GLfloat     width;          // [0..50] px
};

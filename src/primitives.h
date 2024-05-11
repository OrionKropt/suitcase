#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

class Primitive
{
public:
    virtual ~Primitive() = 0;
};

class Point : public Primitive
{
public:

};

class Line : public Primitive
{
public:
    Line(glm::vec2 start, glm::vec2 end, GLfloat width = 7.0f, glm::vec3 color = glm::vec3(1.0f, 0.0f, 0.0f));
    Line(GLfloat start_x, GLfloat start_y, GLfloat end_x, GLfloat end_y, GLfloat width = 7.0f, GLfloat R = 1.0f, GLfloat G = 0.0f, GLfloat B = 0.0f);
    ~Line() override;

    auto draw() -> void;
    auto set_color(glm::vec3 new_color) -> void;
    auto set_width(GLfloat new_width) -> void;
    auto set_shader_program(GLuint new_shader_program) -> void;

private:
    GLuint      shader_program;
    GLuint      VAO, VBO;
    glm::vec2   start;
    glm::vec2   end;
    glm::vec3   color;          // [0..1] RGB
    GLfloat     width;          // [0..50] px
};

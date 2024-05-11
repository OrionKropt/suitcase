#include "primitives.h"

#include <glm/gtc/type_ptr.hpp>
#include "opengl.h"

OpenGL& opengl = OpenGL::get_instance();

Primitive::~Primitive() = default;

// Point
// ...

Line::Line(glm::vec2 start, glm::vec2 end, GLfloat width, glm::vec3 color)
{
    this->start = start;
    this->end   = end;
    this->color = color;
    this->width = width;

    shader_program = opengl.get_shader_program("line");

    GLfloat vertices[] = {
            start.x, start.y,
            end.x,   end.y
    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*) 0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

Line::Line(GLfloat start_x, GLfloat start_y, GLfloat end_x, GLfloat end_y, GLfloat width, GLfloat R, GLfloat G, GLfloat B)
    : Line(glm::vec2(start_x, start_y), glm::vec2(end_x, end_y), width, glm::vec3(R, G, B)) {}

Line::~Line()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shader_program);
}

auto Line::draw() -> void
{
    glLineWidth(width);

    glUseProgram(shader_program);
    GLint color_uniform_location = glGetUniformLocation(shader_program, "color");
    glUniform3fv(color_uniform_location, 1, glm::value_ptr(color));

    glBindVertexArray(VAO);
    glDrawArrays(GL_LINES, 0, 2);
}

auto Line::set_color(glm::vec3 new_color) -> void
{
    color = new_color;
}

auto Line::set_width(GLfloat new_width) -> void
{
    width = new_width;
}

auto Line::set_shader_program(GLuint new_shader_program) -> void
{
    shader_program = new_shader_program;
}

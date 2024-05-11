#include "primitives.h"

#include <glm/gtc/type_ptr.hpp>
#include "opengl.h"

OpenGL& opengl = OpenGL::get_instance();

Primitive::~Primitive() = default;


Point::Point(glm::vec2 point, GLfloat width, glm::vec3 color)
{
    this->point = point;
    this->width = width;
    this->color = color;

    shader_program = opengl.get_shader_program("primitives");

    GLfloat vertices[] = {
            point.x - width, point.y - width,
            point.x - width, point.y + width,
            point.x + width, point.y + width,
            point.x + width, point.y - width
    };

    GLuint indices[] = {
            0, 1, 2,
            0, 2, 3
    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*) 0);
    glEnableVertexAttribArray(0);
}

Point::Point(GLfloat point_x, GLfloat point_y, GLfloat width, GLfloat R, GLfloat G, GLfloat B)
    : Point(glm::vec2(point_x, point_y), width, glm::vec3(R, G, B)) {}

Point::~Point()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

auto Point::draw() -> void
{
    glUseProgram(shader_program);

    GLint color_uniform_location = glGetUniformLocation(shader_program, "color");
    glUniform3fv(color_uniform_location, 1, glm::value_ptr(color));

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

auto Point::set_color(glm::vec3 new_color) -> void
{
    color = new_color;
}

auto Point::set_color(GLfloat R, GLfloat G, GLfloat B) -> void
{
    set_color(glm::vec3(R, G, B));
}

auto Point::set_width(GLfloat new_width) -> void
{
    width = new_width;
}

auto Point::set_shader_program(GLuint new_shader_program) -> void
{
    shader_program = new_shader_program;
}

auto Point::set_shader_program(const char* program_name) -> void
{
    GLuint id = opengl.get_shader_program(program_name);
    set_shader_program(id);
}


Line::Line(glm::vec2 start, glm::vec2 end, GLfloat width, glm::vec3 color)
{
    this->start = start;
    this->end   = end;
    this->color = color;
    this->width = width;

    shader_program = opengl.get_shader_program("primitives");

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

auto Line::set_color(GLfloat R, GLfloat G, GLfloat B) -> void
{
    set_color(glm::vec3(R, G, B));
}

auto Line::set_width(GLfloat new_width) -> void
{
    width = new_width;
}

auto Line::set_shader_program(GLuint new_shader_program) -> void
{
    shader_program = new_shader_program;
}

auto Line::set_shader_program(const char* program_name) -> void
{
    GLuint id = opengl.get_shader_program(program_name);
    set_shader_program(id);
}

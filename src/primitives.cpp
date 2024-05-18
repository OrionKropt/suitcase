#include "primitives.h"

#include <iostream>
#include <glm/gtc/type_ptr.hpp>

OpenGL& opengl = OpenGL::get_instance();

Primitive::~Primitive() = default;
auto Primitive::draw() -> void {}


Point::Point(glm::vec2 position, GLfloat width, glm::vec3 color)
{
    this->position = position;
    this->width = width;
    this->color = color;
    this->shader = opengl.get_shader("primitives");

    GLfloat vertices[] = {
            position.x - width, position.y - width,
            position.x - width, position.y + width,
            position.x + width, position.y + width,
            position.x + width, position.y - width
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

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
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
    shader->use();
    shader->set_vec3("color", color);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

auto Point::move(GLfloat dx, GLfloat dy) -> void
{
    set_position(position.x + dx, position.y + dy);
}

auto Point::set_position(glm::vec2 new_position) -> void
{
    if (position == new_position)
    {
        return;
    }

    position = new_position;

    GLfloat vertices[] = {
            new_position.x - width, new_position.y - width,
            new_position.x - width, new_position.y + width,
            new_position.x + width, new_position.y + width,
            new_position.x + width, new_position.y - width
    };

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

auto Point::set_position(GLfloat new_x, GLfloat new_y) -> void
{
    set_position(glm::vec2(new_x, new_y));
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

auto Point::set_shader(Shader* new_shader) -> void
{
    shader = new_shader;
}

auto Point::set_shader(const char* shader_name) -> void
{
    shader = opengl.get_shader(shader_name);
}


Line::Line(glm::vec2 start, glm::vec2 end, GLfloat width, glm::vec3 color)
{
    this->start = start;
    this->end   = end;
    this->color = color;
    this->width = width;

    shader = opengl.get_shader("primitives");

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
    shader->use();
    shader->set_vec3("color", color);
    glBindVertexArray(VAO);
    glDrawArrays(GL_LINES, 0, 2);
    glBindVertexArray(0);
}

auto Line::move(GLfloat dx, GLfloat dy) -> void
{
    set_position(start.x + dx, start.y + dy, end.x + dx, end.y + dy);
}

auto Line::set_position(glm::vec2 new_start, glm::vec2 new_end) -> void
{
    if (start == new_start && end == new_end)
    {
        return;
    }

    start = new_start;
    end = new_end;

    GLfloat vertices[] = {
            new_start.x, new_start.y,
            new_end.x,   new_end.y
    };

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

auto Line::set_position(GLfloat new_start_x, GLfloat new_start_y, GLfloat new_end_x, GLfloat new_end_y) -> void
{
    set_position(glm::vec2(new_start_x, new_start_y), glm::vec2(new_end_x, new_end_y));
}

auto Line::set_start(glm::vec2 new_start) -> void
{
    if (start == new_start)
    {
        return;
    }

    start = new_start;

    GLfloat vertices[] = {
            new_start.x, new_start.y,
            end.x,       end.y
    };

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

auto Line::set_start(GLfloat new_start_x, GLfloat new_start_y) -> void
{
    set_start(glm::vec2(new_start_x, new_start_y));
}

auto Line::set_end(glm::vec2 new_end) -> void
{
    if (end == new_end)
    {
        return;
    }

    end = new_end;

    GLfloat vertices[] = {
            start.x,   start.y,
            new_end.x, new_end.y
    };

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

auto Line::set_end(GLfloat new_end_x, GLfloat new_end_y) -> void
{
    set_end(glm::vec2(new_end_x, new_end_y));
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

auto Line::set_shader(Shader* new_shader) -> void
{
    shader = new_shader;
}

auto Line::set_shader(const char* shader_name) -> void
{
    shader = opengl.get_shader(shader_name);
}


Text::Text(const char* text, glm::vec2 position, GLfloat size, glm::vec3 color)
{
    this->text     = text;
    this->position = position;
    this->size     = size;
    this->color    = color;

    shader = opengl.get_shader("text");

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, nullptr, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

Text::Text(const char* text, GLfloat x, GLfloat y, GLfloat size, GLfloat R, GLfloat G, GLfloat B)
    : Text(text, glm::vec2(x, y), size, glm::vec3(R, G, B)) {}

Text::~Text()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

auto Text::draw() -> void
{
    shader->use();
    shader->set_vec3("text_color", color);

    glm::mat4 projection = glm::ortho(0.0f, (GLfloat) opengl.get_window_width(),
                                      0.0f, (GLfloat) opengl.get_window_height());
    shader->set_mat4("projection", projection);

    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(VAO);

    GLfloat x = position.x;
    GLfloat y = position.y;

    for (const auto& c : text)
    {
        auto ch = opengl.get_char(c);  // auto cause Character is private OpenGL structure

        GLfloat x_pos = x + (GLfloat) ch->bearing.x * size;
        GLfloat y_pos = y - (GLfloat) (ch->size.y - ch->bearing.y) * size;

        GLfloat width  = (GLfloat) ch->size.x * size;
        GLfloat height = (GLfloat) ch->size.y * size;

        GLfloat vertices[] = {
                x_pos,          y_pos + height,     0.0f, 0.0f,
                x_pos,          y_pos,              0.0f, 1.0f,
                x_pos + width,  y_pos,              1.0f, 1.0f,

                x_pos,          y_pos + height,     0.0f, 0.0f,
                x_pos + width,  y_pos,              1.0f, 1.0f,
                x_pos + width,  y_pos + height,     1.0f, 0.0f
        };

        glBindTexture(GL_TEXTURE_2D, ch->texture);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glDrawArrays(GL_TRIANGLES, 0, 6);

        x += (GLfloat) (ch->advance >> 6) * size;
    }
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

auto Text::move(GLfloat dx, GLfloat dy) -> void
{
    set_position(position.x + dx, position.y + dy);
}

auto Text::set_position(glm::vec2 new_position) -> void
{
    if (position == new_position)
    {
        return;
    }

    position = new_position;
}

auto Text::set_position(GLfloat new_x, GLfloat new_y) -> void
{
    set_position(glm::vec2(new_x, new_y));
}

auto Text::set_text(const char* text) -> void
{
    this->text = text;
}

auto Text::set_color(glm::vec3 new_color) -> void
{
    color = new_color;
}

auto Text::set_color(GLfloat R, GLfloat G, GLfloat B) -> void
{
    color = glm::vec3(R, G, B);
}

auto Text::set_size(GLfloat new_size) -> void
{
    size = new_size;
}

auto Text::set_shader(Shader* new_shader) -> void
{
    shader = new_shader;
}

auto Text::set_shader(const char* shader_name) -> void
{
    shader = opengl.get_shader(shader_name);
}

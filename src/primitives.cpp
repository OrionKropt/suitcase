#include "primitives.h"

#include <format>
#include <iostream>
#include <glm/gtc/type_ptr.hpp>

OpenGL& opengl = OpenGL::get_instance();

Primitive::~Primitive() = default;
auto Primitive::draw() -> void {}


Triangle::Triangle(glm::vec2 v1, glm::vec2 v2, glm::vec2 v3, glm::vec3 color)
{
    this->v1 = v1;
    this->v2 = v2;
    this->v3 = v3;
    this->color = color;

    position = glm::vec2(
            (std::min(v1.x, std::min(v2.x, v3.x)) + std::max(v1.x, std::max(v2.x, v3.x))) / 2,
            (std::min(v1.y, std::min(v2.y, v3.y)) + std::max(v1.y, std::max(v2.y, v3.y))) / 2
    );

    shader = opengl.get_shader("primitives");

    GLfloat vertices[] = {
            v1.x, v1.y,
            v2.x, v2.y,
            v3.x, v3.y
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

Triangle::Triangle(GLfloat v1_x, GLfloat v1_y, GLfloat v2_x, GLfloat v2_y, GLfloat v3_x, GLfloat v3_y, GLfloat R, GLfloat G, GLfloat B)
    : Triangle(glm::vec2(v1_x, v1_y), glm::vec2(v2_x, v2_y), glm::vec2(v3_x, v3_y), glm::vec3(R, G, B)) {}

Triangle::~Triangle()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

auto Triangle::draw() -> void
{
    shader->use();
    shader->set_vec3("color", color);
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}

auto Triangle::move(GLfloat dx, GLfloat dy) -> void
{
    set_position(position.x + dx, position.y + dy);
}

auto Triangle::set_position(glm::vec2 new_position) -> void
{
    if (position == new_position)
    {
        return;
    }

    v1 = (v1 - position) + new_position;
    v2 = (v2 - position) + new_position;
    v3 = (v3 - position) + new_position;

    position = new_position;

    GLfloat vertices[] = {
            v1.x, v1.y,
            v2.x, v2.y,
            v3.x, v3.y
    };

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

auto Triangle::set_position(GLfloat new_x, GLfloat new_y) -> void
{
    set_position(glm::vec2(new_x, new_y));
}

auto Triangle::set_color(glm::vec3 new_color) -> void
{
    color = new_color;
}

auto Triangle::set_color(GLfloat R, GLfloat G, GLfloat B) -> void
{
    set_color(glm::vec3(R, G, B));
}

auto Triangle::set_shader(Shader* new_shader) -> void
{
    shader = new_shader;
}

auto Triangle::set_shader(const char* shader_name) -> void
{
    shader = opengl.get_shader(shader_name);
}


Point::Point(glm::vec2 position, GLfloat width, glm::vec3 color)
{
    this->position = position;
    this->width    = width;
    this->color    = color;

    shader = opengl.get_shader("primitives");

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

    shader = opengl.get_shader("line");

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
    shader->set_float("line_width", width);
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


Text::Text(const char* text, glm::vec2 position, GLfloat size, GLfloat rotation, glm::vec3 color)
{
    this->text     = text;
    this->position = position;
    this->size     = size;
    this->rotation = rotation;
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

    text_width_px  = 0;
    text_height_px = 0;
    for (const auto& c : this->text)
    {
        auto ch = opengl.get_char(c);
        text_width_px  += (ch->advance >> 6) * size;
        GLfloat tmp_height = ch->bearing.y * size;
        if (tmp_height > text_height_px)
        {
            text_height_px = tmp_height;
        }
    }
    text_width_ndc  = (2 * text_width_px) / opengl.get_window_width();
    text_height_ndc = (2 * text_height_px) / opengl.get_window_height();
}

Text::Text(const char* text, GLfloat x, GLfloat y, GLfloat size, GLfloat rotation, GLfloat R, GLfloat G, GLfloat B)
    : Text(text, glm::vec2(x, y), size, rotation, glm::vec3(R, G, B)) {}

Text::~Text()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

auto Text::draw() -> void
{
    GLfloat window_width  = opengl.get_window_width();
    GLfloat window_height = opengl.get_window_height();

    GLfloat x_ndc_to_px = (position.x + 1.0f) / 2.0f * window_width;
    GLfloat y_ndc_to_px = (position.y + 1.0f) / 2.0f * window_height;

    shader->use();
    shader->set_vec3("text_color", color);

    // I fucking hate these calculations. I hate them with all my soul and heart.
    glm::mat4 projection = glm::ortho(0.0f, window_width, 0.0f, window_height);
    glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(x_ndc_to_px, y_ndc_to_px, 0.0f));
    model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::translate(model, glm::vec3(-x_ndc_to_px, -y_ndc_to_px, 0.0f));

    shader->set_mat4("projection", projection);
    shader->set_mat4("model", model);

    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(VAO);

    GLfloat x = x_ndc_to_px;
    GLfloat y = y_ndc_to_px;

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

auto Text::get_width_px() -> GLfloat
{
    return text_width_px;
}

auto Text::get_height_px() -> GLfloat
{
    return text_height_px;
}

auto Text::get_width_ndc() -> GLfloat
{
    return text_width_ndc;
}

auto Text::get_height_ndc() -> GLfloat
{
    return text_height_ndc;
}

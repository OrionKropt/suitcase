#pragma once

#include <string>
#include <unordered_map>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "opengl.h"

class Primitive
{
public:
    virtual ~Primitive() = 0;
    virtual auto draw() -> void = 0;
};


class Triangle : public Primitive
{
public:
    Triangle(glm::vec2 v1, glm::vec2 v2, glm::vec2 v3, glm::vec3 color = glm::vec3(1.0f, 0.0f, 0.0f));
    Triangle(GLfloat v1_x, GLfloat v1_y, GLfloat v2_x, GLfloat v2_y, GLfloat v3_x, GLfloat v3_y, GLfloat R = 1.0f, GLfloat G = 0.0f, GLfloat B = 0.0f);
    ~Triangle() override;

    auto draw() -> void override;
    auto move(glm::vec2 delta) -> void;
    auto move(GLfloat dx, GLfloat dy) -> void;
    auto get_position() -> glm::vec2;
    auto set_position(glm::vec2 new_position) -> void;
    auto set_position(GLfloat new_x, GLfloat new_y) -> void;
    auto set_color(glm::vec3 new_color) -> void;
    auto set_color(GLfloat R, GLfloat G, GLfloat B) -> void;
    auto set_shader(std::shared_ptr<Shader> new_shader) -> void;
    auto set_shader(const char* shader_name) -> void;

private:
    std::shared_ptr<Shader> shader;
    GLuint                  VAO;
    GLuint                  VBO;
    glm::vec2               v1;                 // NDC
    glm::vec2               v2;                 // NDC
    glm::vec2               v3;                 // NDC
    glm::vec2               position;           // NDC
    glm::vec3               color;
};


class Point : public Primitive
{
public:
    explicit Point(glm::vec2 position, GLfloat width = 0.015f, glm::vec3 color = glm::vec3(1.0f, 0.0f, 0.0f));
    Point(GLfloat point_x, GLfloat point_y, GLfloat width = 0.015f, GLfloat R = 1.0f, GLfloat G = 0.0f, GLfloat B = 0.0f);
    ~Point() override;

    auto draw() -> void override;
    auto move(glm::vec2 delta) -> void;
    auto move(GLfloat dx, GLfloat dy) -> void;
    auto get_position() -> glm::vec2;
    auto set_position(glm::vec2 new_position) -> void;
    auto set_position(GLfloat new_x, GLfloat new_y) -> void;
    auto set_color(glm::vec3 new_color) -> void;
    auto set_color(GLfloat R, GLfloat G, GLfloat B) -> void;
    auto set_width(GLfloat new_width) -> void;
    auto set_shader(std::shared_ptr<Shader> new_shader) -> void;
    auto set_shader(const char* shader_name) -> void;

private:
    std::shared_ptr<Shader> shader;
    GLuint                  VAO;
    GLuint                  VBO;
    GLuint                  EBO;
    glm::vec2               position;           // NDC
    glm::vec3               color;
    GLfloat                 width;
};


class Line : public Primitive
{
public:
    Line(glm::vec2 start, glm::vec2 end, GLfloat width = 0.015f, glm::vec3 color = glm::vec3(1.0f, 0.0f, 0.0f));
    Line(GLfloat start_x, GLfloat start_y, GLfloat end_x, GLfloat end_y, GLfloat width = 0.05f, GLfloat R = 1.0f, GLfloat G = 0.0f, GLfloat B = 0.0f);
    ~Line() override;

    auto draw() -> void override;
    auto move(glm::vec2 delta) -> void;
    auto move(GLfloat dx, GLfloat dy) -> void;
    auto get_start_position() -> glm::vec2;
    auto get_end_position() -> glm::vec2;
    auto set_position(glm::vec2 new_start, glm::vec2 new_end) -> void;
    auto set_position(GLfloat new_start_x, GLfloat new_start_y, GLfloat new_end_x, GLfloat new_end_y) -> void;
    auto set_start(glm::vec2 new_start) -> void;
    auto set_start(GLfloat new_start_x, GLfloat new_start_y) -> void;
    auto set_end(glm::vec2 new_end) -> void;
    auto set_end(GLfloat new_end_x, GLfloat new_end_y) -> void;
    auto set_color(glm::vec3 new_color) -> void;
    auto set_color(GLfloat R, GLfloat G, GLfloat B) -> void;
    auto set_width(GLfloat new_width) -> void;
    auto set_shader(std::shared_ptr<Shader> new_shader) -> void;
    auto set_shader(const char* shader_name) -> void;

private:
    std::shared_ptr<Shader> shader;
    GLuint                  VAO;
    GLuint                  VBO;
    glm::vec2               start;          // NDC
    glm::vec2               end;            // NDC
    glm::vec3               color;
    GLfloat                 width;          // NDC
};


class Text : public Primitive
{
public:
    Text(const char* text, glm::vec2 position, GLfloat size, GLfloat rotation = 0.0f, glm::vec3 color = glm::vec3(0.0f, 0.0f, 0.0f));
    Text(const char* text, GLfloat x, GLfloat y, GLfloat size, GLfloat rotation = 0.0f, GLfloat R = 0.0f, GLfloat G = 0.0f, GLfloat B = 0.0f);
    ~Text() override;

    auto draw() -> void override;
    auto move(glm::vec2 delta) -> void;
    auto move(GLfloat dx, GLfloat dy) -> void;
    auto get_position() -> glm::vec2;
    auto set_position(glm::vec2 new_position) -> void;
    auto set_position(GLfloat new_x, GLfloat new_y) -> void;
    auto set_text(const char* text) -> void;
    auto set_color(glm::vec3 new_color) -> void;
    auto set_color(GLfloat R, GLfloat G, GLfloat B) -> void;
    auto set_size(GLfloat new_size) -> void;
    auto set_shader(std::shared_ptr<Shader> new_shader) -> void;
    auto set_shader(const char* shader_name) -> void;
    auto get_width_px() -> GLfloat;
    auto get_height_px() -> GLfloat;
    auto get_width_ndc() -> GLfloat;
    auto get_height_ndc() -> GLfloat;

private:
    auto calculate_text_size() -> void;

    std::shared_ptr<Shader> shader;
    std::string             text;
    GLuint                  VAO;
    GLuint                  VBO;
    glm::vec2               position;           // NDC
    glm::vec3               color;
    GLfloat                 size;
    GLfloat                 rotation;           // Degrees
    GLfloat                 text_width_px;      // px
    GLfloat                 text_height_px;     // px
    GLfloat                 text_width_ndc;     // NDC
    GLfloat                 text_height_ndc;    // NDC
};


class Button : public Primitive
{
public:
    Button(glm::vec2 position, GLfloat width, GLfloat height,
           void (*on_press)(), void (*on_release)(), const char* text = nullptr,
           GLfloat text_size = 0.4f, glm::vec3 color = glm::vec3(0.9f, 0.9f, 0.9f));

    Button(GLfloat x, GLfloat y, GLfloat width, GLfloat height,
           void (*on_press)(), void (*on_release)(), const char* text = nullptr,
           GLfloat text_size = 0.4f, GLfloat R = 0.9f, GLfloat G = 0.9f, GLfloat B = 0.9f);

    ~Button() override;

    auto draw() -> void override;
    auto set_on_press(void (*callback)()) -> void;
    auto set_on_release(void (*callback)()) -> void;
    auto set_color(glm::vec3 new_color) -> void;
    auto set_color(GLfloat R, GLfloat G, GLfloat B) -> void;

private:
    auto check_pressing() -> void;

    // Fun fact: when lmb_* vars were static click detection
    // worked only for first created button. Why? IDK
    GLboolean               lmb_pressed;
    GLboolean               lmb_hold;
    GLboolean               button_pressed;
    GLboolean               button_hovered;
    GLFWwindow*             window;
    GLfloat                 left_border;
    GLfloat                 right_border;
    GLfloat                 up_border;
    GLfloat                 down_border;
    std::shared_ptr<Shader> shader;

    GLuint                  VAO;
    GLuint                  VBO;
    GLuint                  EBO;
    glm::vec2               position;           // NDC
    GLfloat                 width;              // NDC
    GLfloat                 height;             // NDC
    glm::vec3               color;
    void                    (*on_press)();
    void                    (*on_release)();
    std::shared_ptr<Text>   text;
};

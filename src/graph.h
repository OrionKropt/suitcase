#pragma once

#include <chrono>
#include <string>
#include <vector>
#include <variant>
#include <glm/glm.hpp>
#include "primitives.h"

class Graph
{
public:
    using TimePoint = std::chrono::time_point<std::chrono::system_clock>;
    using AxisValue = std::variant<GLfloat, GLint64, TimePoint>;

    Graph(const char* alternative, const char* ordinate, AxisValue hor_zero_value, AxisValue ver_zero_value,
          GLfloat hor_value_step, GLfloat ver_value_step, GLint hor_delims, GLint ver_delims,
          GLint hor_center = 0, GLint ver_center = 0, GLint hor_value_skip = 0,
          GLint ver_value_skip = 0, GLfloat hor_grid_step = 0.05f, GLfloat ver_grid_step = 0.05f,
          glm::vec2 position = glm::vec2(0.0f, 0.0f));
    ~Graph();

    auto draw() -> void;
    auto add_point(AxisValue x, AxisValue y) -> void;
    auto add_segment(glm::vec2 start, glm::vec2 end) -> void;
    auto set_axis_color(glm::vec3 new_color) -> void;
    auto set_axis_color(GLfloat R, GLfloat G, GLfloat B) -> void;
    auto set_grid_color(glm::vec3 new_color) -> void;
    auto set_grid_color(GLfloat R, GLfloat G, GLfloat B) -> void;

private:
    auto initialize_axis_texts(std::vector<AxisValue>& values, AxisValue& zero_value, GLint delims, GLint center, GLfloat value_step) -> void;
    auto get_raw_text(AxisValue variant) -> std::string;
    auto find_value(GLint axis, AxisValue axis_value) -> GLfloat;
    auto create_next_value(AxisValue& current, GLfloat step) -> AxisValue;
    auto update_labels() -> void;

    std::string abscissa;
    std::string ordinate;
    AxisValue   hor_zero_value;
    AxisValue   ver_zero_value;
    GLfloat     hor_value_step;
    GLfloat     ver_value_step;
    GLint       hor_delims;
    GLint       ver_delims;
    GLfloat     hor_grid_step;          // NDC
    GLfloat     ver_grid_step;          // NDC
    GLint       hor_center;
    GLint       center;
    glm::vec2   position;               // NDC

    GLfloat     left_border;
    GLfloat     right_border;
    GLfloat     up_border;
    GLfloat     down_border;
    glm::vec3   bg_color;
    glm::vec3   axis_color;
    GLfloat     bg_lines_width;
    GLfloat     main_lines_width;
    GLfloat     hor_min;
    GLfloat     hor_max;
    GLfloat     ver_min;
    GLfloat     ver_max;
    GLfloat     hor_rotation_deg;
    GLfloat     hor_rotation_rad;

    std::vector<AxisValue> hor_values;
    std::vector<AxisValue> ver_values;

    std::vector<Line*>      bg_lines_hor;
    std::vector<Line*>      bg_lines_ver;
    std::vector<Line*>      main_lines;
    std::vector<Triangle*>  arrows;
    std::vector<Line*>      segments;
    std::vector<Point*>     points;
    std::vector<Text*>      hor_texts;
    std::vector<Text*>      ver_texts;
    std::vector<Text*>      axis_labels;
};

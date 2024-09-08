#pragma once

#include <chrono>
#include <memory>
#include <string>
#include <vector>
#include <variant>
#include <unordered_map>
#include <glm/glm.hpp>
#include "primitives.h"

class Graph
{
public:
    using TimePoint = std::chrono::time_point<std::chrono::system_clock>;
    using AxisValue = std::variant<GLfloat, GLint64, TimePoint>;

    // TODO: Add functional for *_value_skip AND scaling on not 1:1 resolution
    Graph(const wchar_t* abscissa, const wchar_t* ordinate, AxisValue hor_zero_value, AxisValue ver_zero_value,
          GLfloat hor_value_step, GLfloat ver_value_step, GLint hor_delims, GLint ver_delims,
          GLint hor_center = 0, GLint ver_center = 0, GLint hor_value_skip = 0,
          GLint ver_value_skip = 0, GLfloat hor_grid_step = 0.05f, GLfloat ver_grid_step = 0.05f,
          glm::vec2 position = glm::vec2(0.0f, 0.0f));
    ~Graph();

    auto draw() -> void;
    auto create_curve(const char* name, glm::vec3 color = glm::vec3(1.0f, 0.0f, 0.0f)) -> void;
    auto create_curve(const char* name, GLfloat R = 1.0f, GLfloat G = 0.0f, GLfloat B = 0.0f) -> void;
    auto add_point(const char* curve_name, AxisValue x, AxisValue y) -> void;
    auto add_segment(const char* curve_name, glm::vec2 start, glm::vec2 end) -> void;
    auto move(glm::vec2 delta) -> void;
    auto move(GLfloat dx, GLfloat dy) -> void;
    auto set_position(glm::vec2 new_position) -> void;
    auto set_position(GLfloat new_x, GLfloat new_y) -> void;
    auto set_enabled(bool state) -> void;
    auto enable() -> void;
    auto disable() -> void;
    auto is_enabled() const -> bool;
    auto set_axis_color(glm::vec3 new_color) -> void;
    auto set_axis_color(GLfloat R, GLfloat G, GLfloat B) -> void;
    auto set_grid_color(glm::vec3 new_color) -> void;
    auto set_grid_color(GLfloat R, GLfloat G, GLfloat B) -> void;

private:
    auto initialize_axis_texts(std::vector<AxisValue>& values, AxisValue& zero_value, GLint delims, GLint center, GLfloat value_step) -> void;
    auto get_raw_text(AxisValue variant) -> std::wstring;
    auto find_value(GLint axis, AxisValue axis_value) -> GLfloat;
    auto create_next_value(AxisValue& current, GLfloat step) -> AxisValue;
    auto update_labels() -> void;

    bool        enabled;                // Should Graph be rendered

    std::wstring abscissa;               // Labels for axis'
    std::wstring ordinate;
    AxisValue   hor_zero_value;         // Values at (0, 0) point
    AxisValue   ver_zero_value;
    GLfloat     hor_value_step;         // Steps between values: value[n] = value[n-1] + step
    GLfloat     ver_value_step;
    GLint       hor_delims;             // Amount of hor/ver lines in grid
    GLint       ver_delims;
    GLfloat     hor_grid_step;          // NDC
    GLfloat     ver_grid_step;          // NDC
    GLint       hor_center;
    GLint       ver_center;
    glm::vec2   position;               // NDC

    GLfloat     left_border;
    GLfloat     right_border;
    GLfloat     up_border;
    GLfloat     down_border;
    glm::vec3   bg_lines_color      = glm::vec3(0.5f, 0.5f, 0.5f);
    glm::vec3   axis_color          = glm::vec3(0.0f, 0.0f, 0.0f);
    GLfloat     bg_lines_width      = 0.0035f;
    GLfloat     main_lines_width    = 0.01f;
    GLfloat     hor_min;
    GLfloat     hor_max;
    GLfloat     ver_min;
    GLfloat     ver_max;
    GLfloat     hor_rotation_deg;
    GLfloat     hor_rotation_rad;

    std::vector<AxisValue> hor_values;
    std::vector<AxisValue> ver_values;

    struct Curve
    {
        explicit Curve(glm::vec3 color);

        glm::vec3                           color{};
        std::vector<std::shared_ptr<Point>> points;
        std::vector<std::shared_ptr<Line>>  segments;
    };

    std::unordered_map<std::string, std::shared_ptr<Curve>> curves;     // Curve name -> curve

    std::vector<std::shared_ptr<Line>>      bg_lines_hor;
    std::vector<std::shared_ptr<Line>>      bg_lines_ver;
    std::vector<std::shared_ptr<Line>>      main_lines;
    std::vector<std::shared_ptr<Triangle>>  arrows;
    std::vector<std::shared_ptr<Text>>      hor_texts;
    std::vector<std::shared_ptr<Text>>      ver_texts;
    std::vector<std::shared_ptr<Text>>      axis_labels;
};

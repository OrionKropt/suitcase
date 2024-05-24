#include "graph.h"

#include <format>
#include <sstream>
#include <iostream>
#include "error.h"

extern OpenGL& opengl;


Graph::Graph(const char* abscissa, const char* ordinate, AxisValue hor_zero_value, AxisValue ver_zero_value,
             GLfloat hor_value_step, GLfloat ver_value_step, GLint hor_delims, GLint ver_delims,
             GLint hor_center, GLint ver_center, GLint hor_value_skip, GLint ver_value_skip,
             GLfloat hor_grid_step, GLfloat ver_grid_step, glm::vec2 position)
{
    this->abscissa      = abscissa;
    this->ordinate      = ordinate;
    this->hor_delims    = hor_delims;
    this->ver_delims    = ver_delims;
    this->hor_grid_step = hor_grid_step;
    this->ver_grid_step = ver_grid_step;
    this->position      = position;
    this->hor_center    = hor_center;
    this->center    = ver_center;

    bg_color         = glm::vec3(0.5f, 0.5f, 0.5f);
    axis_color       = glm::vec3(0.0f, 0.0f, 0.0f);
    bg_lines_width   = 0.0035f;
    main_lines_width = 0.01f;

    GLfloat hor_offset = (hor_delims % 2 ? 0 : hor_grid_step / 2);
    GLfloat ver_offset = (ver_delims % 2 ? 0 : ver_grid_step / 2);

    left_border  = -hor_grid_step * (hor_delims / 2) + hor_offset;
    right_border = hor_grid_step * (hor_delims / 2) - hor_offset;
    up_border    = ver_grid_step * (ver_delims / 2) - ver_offset;
    down_border  = -ver_grid_step * (ver_delims / 2) + ver_offset;

    // * Background lines (grid)
    bg_lines_hor = std::vector<Line*>(hor_delims);
    GLfloat tmp_left = left_border;
    for (int i = 0; i < hor_delims; ++i) {
        auto* bg_line = new Line(glm::vec2(tmp_left, down_border),
                                 glm::vec2(tmp_left, up_border),
                                 bg_lines_width, bg_color);
        bg_lines_hor[i] = bg_line;

        tmp_left += hor_grid_step;
    }

    bg_lines_ver = std::vector<Line*>(ver_delims);
    GLfloat tmp_down = down_border;
    for (int i = 0; i < ver_delims; ++i) {
        auto* bg_line = new Line(glm::vec2(left_border, tmp_down),
                                 glm::vec2(right_border, tmp_down),
                                 bg_lines_width, bg_color);
        bg_lines_ver[i] = bg_line;
        tmp_down += ver_grid_step;
    }

    // * Main lines (axis)
    if (abs(ver_center) > floor(hor_delims / 2))
    {
        PRINT_ERROR("Invalid abscissa axis position", true,
                    "{0:} is provided, but the valid range is [-{1:}, {1:}]",
                    ver_center, (GLint) floor(hor_delims / 2));
    }
    if (abs(hor_center) > floor(ver_delims / 2))
    {
        PRINT_ERROR("Invalid ordinate axis position", true,
                    "{0:} is provided, but the valid range is [-{1:}, {1:}]",
                    hor_center, (GLint) floor(ver_delims / 2));
    }

    GLfloat hor_axis_offset = hor_center * ver_grid_step - ver_offset;
    GLfloat ver_axis_offset = ver_center * hor_grid_step - hor_offset;

    main_lines = std::vector<Line*>(2);
    auto* hor_main_line = new Line(glm::vec2(left_border, hor_axis_offset),
                                   glm::vec2(right_border, hor_axis_offset),
                                   main_lines_width, axis_color);
    auto* ver_main_line = new Line(glm::vec2(ver_axis_offset, down_border),
                                   glm::vec2(ver_axis_offset, up_border),
                                   main_lines_width, axis_color);
    main_lines[0] = hor_main_line;
    main_lines[1] = ver_main_line;

    // * Arrows
    arrows = std::vector<Triangle*>(2);
    auto* hor_arrow = new Triangle(glm::vec2(right_border + main_lines_width * 2, hor_axis_offset),
                                   glm::vec2(right_border - main_lines_width * 2, hor_axis_offset - main_lines_width * 2),
                                   glm::vec2(right_border - main_lines_width * 2, hor_axis_offset + main_lines_width * 2),
                                   axis_color);
    auto* ver_arrow = new Triangle(glm::vec2(ver_axis_offset, up_border + main_lines_width * 2),
                                   glm::vec2(ver_axis_offset + main_lines_width * 2, up_border - main_lines_width * 2),
                                   glm::vec2(ver_axis_offset - main_lines_width * 2, up_border - main_lines_width * 2),
                                   axis_color);
    arrows[0] = hor_arrow;
    arrows[1] = ver_arrow;

    // * Points

    // * Segments

    // * Text
    hor_values = std::vector<AxisValue>(hor_delims);
    ver_values = std::vector<AxisValue>(ver_delims);

    // Abscissa values init
    initialize_axis_texts(hor_values, hor_zero_value, hor_delims, ver_center, hor_value_step);

    // Ordinate values init
    initialize_axis_texts(ver_values, ver_zero_value, ver_delims, hor_center, ver_value_step);

    // Horizontal texts
    hor_texts = std::vector<Text*>(hor_delims);
    for (int i = 0; i < hor_delims; ++i)
    {
        std::string raw_text = get_raw_text(hor_values[i]);
        auto* text = new Text(raw_text.c_str(),
                              glm::vec2(left_border + i * hor_grid_step,
                                                down_border - ver_grid_step / 2),
                              0.4f, -55.0f);    // ! Hardcoded rotation and size
        text->move(-text->get_height_ndc() / 2, 0.0f);
        hor_texts[i] = text;
    }

    // Vertical texts
    ver_texts = std::vector<Text*>(ver_delims);
    for (int i = 0; i < ver_delims; ++i)
    {
        std::string raw_text = get_raw_text(ver_values[i]);
        auto* text = new Text(raw_text.c_str(),
                              glm::vec2(left_border - hor_grid_step / 2,
                                                down_border + i * ver_grid_step),
                              0.4f, 0.0f);    // ! Hardcoded rotation and size
        text->move(-text->get_width_ndc(), -text->get_height_ndc() / 2);
        ver_texts[i] = text;
    }
}

Graph::~Graph()
{
    for (auto& ptr : points) {
        delete ptr;
    }
    for (auto& ptr : segments) {
        delete ptr;
    }
    for (auto& ptr : bg_lines_hor) {
        delete ptr;
    }
    for (auto& ptr : bg_lines_ver) {
        delete ptr;
    }
    for (auto& ptr : main_lines) {
        delete ptr;
    }
}

auto Graph::draw() -> void
{
    // * Render order to avoid incorrect overlaying:
    // * bg_lines -> main_lines -> arrows -> segments -> points -> text
    for (const auto& bg_line : bg_lines_hor)
    {
        bg_line->draw();
    }
    for (const auto& bg_line : bg_lines_ver)
    {
        bg_line->draw();
    }
    for (const auto& main_line : main_lines)
    {
        main_line->draw();
    }
    for (const auto& arrow : arrows)
    {
        arrow->draw();
    }
    for (const auto& point : points)
    {

    }
    for (const auto& segment : segments)
    {

    }
    for (const auto& hor_text : hor_texts)
    {
        hor_text->draw();
    }
    for (const auto& ver_text : ver_texts)
    {
        ver_text->draw();
    }
}

auto Graph::add_point(AxisValue x, AxisValue y) -> void
{

}

auto Graph::set_axis_color(glm::vec3 new_color) -> void
{
    for (const auto& main_line : main_lines)
    {
        main_line->set_color(new_color);
    }
    for (const auto& arrow : arrows)
    {
        arrow->set_color(new_color);
    }
}

auto Graph::set_axis_color(GLfloat R, GLfloat G, GLfloat B) -> void
{
    set_axis_color(glm::vec3(R, G, B));
}

auto Graph::set_grid_color(glm::vec3 new_color) -> void
{
    for (const auto& bg_line : bg_lines_hor)
    {
        bg_line->set_color(new_color);
    }
    for (const auto& bg_line : bg_lines_ver)
    {
        bg_line->set_color(new_color);
    }
}

auto Graph::set_grid_color(GLfloat R, GLfloat G, GLfloat B) -> void
{
    set_grid_color(glm::vec3(R, G, B));
}

auto Graph::initialize_axis_texts(std::vector<AxisValue>& values, AxisValue& zero_value, GLint delims, GLint center, GLfloat value_step) -> void
{
    std::visit([&](auto&& alternative) -> void
               {
                   GLint range = floor(delims / 2);
                   GLint range_offset = (center < 0 && delims % 2 == 0) ? 1 : 0;
                   GLint begin = (-range + range_offset) - center;
                   GLint end = range - center;

                   using Type = std::decay_t<decltype(alternative)>;
                   if constexpr (std::is_same_v<Type, GLfloat>)
                   {
                       for (int i = begin, j = 0; i <= end; ++i, ++j)
                       {
                           values[j] = alternative + i * value_step;
                       }
                   }
                   else if constexpr (std::is_same_v<Type, GLint64>)
                   {
                       for (int i = begin, j = 0; i <= end; ++i, ++j)
                       {
                           values[j] = alternative + i * static_cast<GLint64>(value_step);
                       }
                   }
                   else if constexpr (std::is_same_v<Type, TimePoint>)
                   {
                       auto duration = alternative.time_since_epoch();
                       GLint64 value = std::chrono::duration_cast<std::chrono::seconds>(duration).count();
                       for (int i = begin, j = 0; i <= end; ++i, ++j)
                       {
                           values[j] = std::chrono::system_clock::from_time_t(value + i * (GLint64) value_step);
                       }
                   }
               }, zero_value);
}

auto Graph::get_raw_text(AxisValue variant) -> std::string
{
    return std::visit(
            [](auto&& alternative) -> std::string
            {
                using Type = std::decay_t<decltype(alternative)>;
                std::string str;
                if constexpr (std::is_same_v<Type, GLfloat>)
                {
                    str = std::format("{:.2f}", alternative);
                }
                else if constexpr (std::is_same_v<Type, GLint64>)
                {
                    str = std::to_string(alternative);
                }
                else if constexpr (std::is_same_v<Type, TimePoint>)
                {
                    auto duration = alternative.time_since_epoch();
                    GLint64 value = std::chrono::duration_cast<std::chrono::seconds>(duration).count();
                    std::stringstream ss;
                    ss << std::put_time(std::localtime(&value), "%H:%M:%S");
                    str = ss.str();
                }
                return str;
            }, variant);
}

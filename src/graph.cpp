#include "graph.h"

#include <iostream>

Graph::Graph(const char* abscissa, const char* ordinate, GLint hor_delims, GLint ver_delims,
             GLint hor_center, GLint ver_center, glm::vec2 position, GLfloat hor_step, GLfloat ver_step)
{
    this->abscissa   = abscissa;
    this->ordinate   = ordinate;
    this->hor_delims = hor_delims;
    this->ver_delims = ver_delims;
    this->hor_step   = hor_step;
    this->ver_step   = ver_step;
    this->position   = position;
    this->hor_center = hor_center;
    this->ver_center = ver_center;

    bg_color         = glm::vec3(0.5f, 0.5f, 0.5f);
    axis_color       = glm::vec3(0.0f, 0.0f, 0.0f);
    bg_lines_width   = 0.0035f;
    main_lines_width = 0.01f;

    GLfloat hor_offset = (hor_delims % 2 ? 0 : hor_step / 2);
    GLfloat ver_offset = (ver_delims % 2 ? 0 : ver_step / 2);

    left_border  = -hor_step * (hor_delims / 2) + hor_offset;
    right_border =  hor_step * (hor_delims / 2) - hor_offset;
    up_border    =  ver_step * (ver_delims / 2) - ver_offset;
    down_border  = -ver_step * (ver_delims / 2) + ver_offset;

    // * Background lines (grid)
    bg_lines_hor = std::vector<Line*>(hor_delims);
    GLfloat tmp_left = left_border;
    for (int i = 0; i < hor_delims; ++i) {
        auto* bg_line = new Line(glm::vec2(tmp_left, down_border),
                                 glm::vec2(tmp_left, up_border),
                                 bg_lines_width, bg_color);
        bg_lines_hor[i] = bg_line;

        tmp_left += hor_step;
    }

    bg_lines_ver = std::vector<Line*>(ver_delims);
    GLfloat tmp_down = down_border;
    for (int i = 0; i < ver_delims; ++i) {
        auto* bg_line = new Line(glm::vec2(left_border, tmp_down),
                                 glm::vec2(right_border, tmp_down),
                                 bg_lines_width, bg_color);
        bg_lines_ver[i] = bg_line;
        tmp_down += ver_step;
    }

    // * Main lines (axis)
    GLfloat hor_axis_offset = hor_center * ver_step - ver_offset;
    GLfloat ver_axis_offset = ver_center * hor_step - hor_offset;

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
}

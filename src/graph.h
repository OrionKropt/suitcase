#pragma once

#include <string>
#include <vector>
#include <glm/glm.hpp>
#include "primitives.h"

class Graph
{
public:
    Graph(const char* abscissa, const char* ordinate, GLint hor_delims, GLint ver_delims,
          GLint hor_center = 0, GLint ver_center = 0, glm::vec2 position = glm::vec2(0.0f, 0.0f),
          GLfloat hor_step = 0.05f, GLfloat ver_step = 0.05f);
    ~Graph();

    auto draw() -> void;
    auto add_point(GLfloat x, GLfloat y) -> void;

private:
    std::string abscissa;
    std::string ordinate;
    GLint       hor_delims;
    GLint       ver_delims;
    GLfloat     hor_step;           // NDC
    GLfloat     ver_step;           // NDC
    GLint       hor_center;
    GLint       ver_center;
    glm::vec2   position;           // NDC

    GLfloat     left_border;
    GLfloat     right_border;
    GLfloat     up_border;
    GLfloat     down_border;
    glm::vec3   bg_color;
    glm::vec3   axis_color;
    GLfloat     bg_lines_width;
    GLfloat     main_lines_width;

    std::vector<Line*>      bg_lines_hor;
    std::vector<Line*>      bg_lines_ver;
    std::vector<Line*>      main_lines;
    std::vector<Triangle*>  arrows;
    std::vector<Line*>      segments;
    std::vector<Point*>     points;
};

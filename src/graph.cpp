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
    this->abscissa       = abscissa;
    this->ordinate       = ordinate;
    this->hor_zero_value = hor_zero_value;
    this->ver_zero_value = ver_zero_value;
    this->hor_value_step = hor_value_step;
    this->ver_value_step = ver_value_step;
    this->hor_delims     = hor_delims;
    this->ver_delims     = ver_delims;
    this->hor_grid_step  = hor_grid_step;
    this->ver_grid_step  = ver_grid_step;
    this->position       = position;
    this->hor_center     = hor_center;
    this->center         = ver_center;

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

    /*
    // * Points
    points = std::vector<Point*>(hor_delims);

    // * Segments
    segments = std::vector<Line*>(points.size() - 1);
    */

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
        hor_rotation_deg = 55.0f;  // ! Hardcode
        hor_rotation_rad = hor_rotation_deg * M_PI / 180.0f;

        std::string raw_text = get_raw_text(hor_values[i]);
        auto* text = new Text(raw_text.c_str(),
                              glm::vec2(left_border + i * hor_grid_step,
                                                down_border - ver_grid_step / 4),
                              0.4f, hor_rotation_deg);
        text->move(-(text->get_width_ndc() * cos(hor_rotation_rad) - text->get_height_ndc() * sin(hor_rotation_rad)),
                   -(text->get_width_ndc() * sin(hor_rotation_rad) + text->get_height_ndc() * cos(hor_rotation_rad)));
        hor_texts[i] = text;
    }

    // Vertical texts
    ver_texts = std::vector<Text*>(ver_delims);
    for (int i = 0; i < ver_delims; ++i)
    {
        // TODO: Refactor like horizontal text above?
        std::string raw_text = get_raw_text(ver_values[i]);
        auto* text = new Text(raw_text.c_str(),
                              glm::vec2(left_border - hor_grid_step / 2,
                                                down_border + i * ver_grid_step),
                              0.4f, 0.0f);    // ! Hardcode
        text->move(-text->get_width_ndc(), -text->get_height_ndc() / 2);
        ver_texts[i] = text;
    }

    // * Axis labels
    axis_labels = std::vector<Text*>(2);
    auto* hor_label = new Text(abscissa,
                               glm::vec2(right_border + hor_grid_step / 1.5f,
                                                 hor_axis_offset),
                               0.65f, 0.0f);
    hor_label->move(0.0f, -hor_label->get_height_ndc() / 2);
    auto* ver_label = new Text(ordinate,
                               glm::vec2(ver_axis_offset,
                                                 up_border + ver_grid_step / 1.5f),
                               0.65f, 0.0f);
    ver_label->move(-ver_label->get_width_ndc() / 2, 0.0f);
    axis_labels[0] = hor_label;
    axis_labels[1] = ver_label;
}

Graph::~Graph()
{
    for (auto& bg_line : bg_lines_hor)
    {
        delete bg_line;
    }
    for (auto& bg_line : bg_lines_ver)
    {
        delete bg_line;
    }
    for (auto& main_line : main_lines)
    {
        delete main_line;
    }
    for (auto& arrow : arrows)
    {
        delete arrow;
    }
    for (auto& segment : segments)
    {
        delete segment;
    }
    for (auto& point : points)
    {
        delete point;
    }
    for (auto& hor_text : hor_texts)
    {
        delete hor_text;
    }
    for (auto& ver_text : ver_texts)
    {
        delete ver_text;
    }
    for (auto& label : axis_labels)
    {
        delete label;
    }
}

auto Graph::draw() -> void
{
    // * Render order to avoid incorrect overlaying:
    // * bg_lines -> main_lines -> arrows -> segments -> points -> text -> labels
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
    for (const auto& segment : segments)
    {
        segment->draw();
    }
    for (const auto& point : points)
    {
        point->draw();
    }
    for (const auto& hor_text : hor_texts)
    {
        hor_text->draw();
    }
    for (const auto& ver_text : ver_texts)
    {
        ver_text->draw();
    }
    for (const auto& label : axis_labels)
    {
        label->draw();
    }
}

auto Graph::add_point(AxisValue x, AxisValue y) -> void
{
    GLfloat x_part = find_value(0, x);
    GLfloat y_part = find_value(1, y);

    if (x_part == -1.0f || y_part == -1.0f)
    {
        PRINT_ERROR("Can't create a point.", false, "No matching coordinate found.\n");
        return;
    }

    if (x_part > 1.0f)
    {
        if (x_part > 2.0f)
        {
            PRINT_ERROR("Can't create a point.", false, "X coordinate goes further than 2 horizontal axis'.\n");
            return;
        }

        GLfloat x_fract = x_part - 1.0f;
        GLint values_to_move = lround(x_fract * (hor_delims - 1));
        GLfloat x_ceil_fract = static_cast<GLfloat>(values_to_move) / (hor_delims - 1);
        x_part = 1.0f - (x_ceil_fract - x_fract);

        AxisValue last_value = *(hor_values.end()-1);
        hor_values.erase(hor_values.begin(), hor_values.begin() + values_to_move);
        for (int i = 0; i < values_to_move; ++i)
        {
            last_value = create_next_value(last_value, hor_value_step);
            hor_values.push_back(last_value);
        }
        update_labels();

        // Shift all points & segments to the left
        for (const auto& point : points)
        {
            point->move(-values_to_move * hor_grid_step, 0.0f);
        }
        for (const auto& segment : segments)
        {
            segment->move(-values_to_move * hor_grid_step, 0.0f);
        }
    }

    GLfloat x_pos = left_border + x_part * (right_border - left_border);
    GLfloat y_pos = down_border + y_part * (up_border - down_border);

    auto* point = new Point(glm::vec2(x_pos, y_pos));
    points.push_back(point);

    // Connect last two points with a segment
    if (points.size() >= 2)
    {
        add_segment((*(points.end()-2))->get_position(), (*(points.end()-1))->get_position());
    }

    // If some points/segments gone further than left border, delete them
    for (auto it = points.begin(); it < points.end();)
    {
        if ((*it)->get_position().x < left_border)
        {
            delete (*it);
            points.erase(it);
            continue;
        }
        it++;
    }

    for (auto it = segments.begin(); it < segments.end();)
    {
        if ((*it)->get_end_position().x <= left_border)
        {
            delete (*it);
            it = segments.erase(it);
            continue;
        }
        // Short the segment if it particularly crosses the border
        else if ((*it)->get_start_position().x < left_border)
        {
            GLfloat tan = ((*it)->get_end_position().y - (*it)->get_start_position().y)
                          / ((*it)->get_end_position().x - (*it)->get_start_position().x);
            GLfloat diff = left_border - (*it)->get_start_position().x;
            (*it)->set_start(glm::vec2(left_border, (*it)->get_start_position().y + diff * tan));
        }
        it++;
    }
}

auto Graph::add_segment(glm::vec2 start, glm::vec2 end) -> void
{
    if (start.x > end.x)
    {
        std::swap(start, end);
    }
    auto* segment = new Line(glm::vec2(start.x, start.y), glm::vec2(end.x, end.y));
    segments.push_back(segment);
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
                       for (int i = begin, j = 0; i <= end; ++i, ++j)
                       {
                           values[j] = alternative + i * std::chrono::seconds(static_cast<GLint64>(value_step));
                       }
                   }
               }, zero_value);
}

auto Graph::get_raw_text(AxisValue variant) -> std::string
{
    return std::visit([](auto&& alternative) -> std::string
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

auto Graph::find_value(GLint axis, AxisValue axis_value) -> GLfloat
{
    std::vector<AxisValue>* values;

    switch (axis) {
        case 0:
            values = &hor_values;
            break;
        case 1:
            values = &ver_values;
            break;
        default:
            PRINT_ERROR("Incorrect axis provided", true);
    }

    GLfloat part = std::visit([&values](auto&& alternative) -> GLfloat
            {
                GLfloat result;
                using Type = std::decay_t<decltype(alternative)>;
                if constexpr (std::is_same_v<Type, GLfloat>)
                {
                    GLfloat offset    = get<GLfloat>(*(values->begin()));
                    GLfloat end_value = get<GLfloat>(*(values->end()-1));
                    result = ((alternative - offset) / (end_value - offset));
                }
                else if constexpr (std::is_same_v<Type, GLint64>)
                {
                    GLint64 offset    = get<GLint64>(*(values->begin()));
                    GLint64 end_value = get<GLint64>(*(values->end()-1));
                    result = (static_cast<GLfloat>(alternative - offset) / static_cast<GLfloat>(end_value - offset));
                }
                else if constexpr (std::is_same_v<Type, TimePoint>)
                {
                    GLint64 offset    = std::chrono::duration_cast<std::chrono::seconds>(get<TimePoint>(*(values->begin())).time_since_epoch()).count();
                    GLint64 end_value = std::chrono::duration_cast<std::chrono::seconds>(get<TimePoint>(*(values->end()-1)).time_since_epoch()).count();
                    GLint64 value     = std::chrono::duration_cast<std::chrono::seconds>(alternative.time_since_epoch()).count();
                    result = (static_cast<GLfloat>(value - offset) / static_cast<GLfloat>(end_value - offset));
                }
                return result;
            }, axis_value);

    return (part >= 0) ? part : -1;
}

auto Graph::create_next_value(AxisValue& current, GLfloat step) -> AxisValue
{
    return std::visit([&step](auto&& alternative) -> AxisValue
            {
                using Type = std::decay_t<decltype(alternative)>;
                AxisValue result;
                if constexpr (std::is_same_v<Type, GLfloat>)
                {
                    result = static_cast<GLfloat>(alternative + step);
                }
                else if constexpr (std::is_same_v<Type, GLint64>)
                {
                    result = static_cast<GLint64>(alternative + step);
                }
                else if constexpr (std::is_same_v<Type, TimePoint>)
                {
                    //GLint64 value = std::chrono::duration_cast<std::chrono::seconds>(alternative.time_since_epoch()).count();
                    //value += step;
                    //result = std::chrono::system_clock::from_time_t(value);
                    result = alternative + std::chrono::seconds(static_cast<GLint64>(step));
                }
                return result;
            }, current);
}

auto Graph::update_labels() -> void {
    for (int i = 0; i < hor_delims; ++i)
    {
        std::string raw_text = get_raw_text(hor_values[i]);
        hor_texts[i]->set_text(raw_text.c_str());
        hor_texts[i]->set_position(glm::vec2(left_border + i * hor_grid_step,
                                             down_border - ver_grid_step / 4));
        hor_texts[i]->move(-(hor_texts[i]->get_width_ndc() * cos(hor_rotation_rad) - hor_texts[i]->get_height_ndc() * sin(hor_rotation_rad)),
                           -(hor_texts[i]->get_width_ndc() * sin(hor_rotation_rad) + hor_texts[i]->get_height_ndc() * cos(hor_rotation_rad)));
    }
}

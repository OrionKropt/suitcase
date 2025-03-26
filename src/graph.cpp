#include "graph.h"

#include <format>
#include <sstream>
#include <iostream>
#include "utils.h"
#include "logger.h"

extern OpenGL& opengl;


Graph::Curve::Curve(glm::vec3 color)
    : color(color) {}

auto Graph::Curve::draw() -> void
{
    for (const auto& point : points)
    {
        point->draw();
    }
    for (const auto& segment : segments)
    {
        segment->draw();
    }
}


auto Graph::Caption::draw() -> void
{
    point->draw();
    text->draw();
}

auto Graph::Caption::move(GLfloat dx, GLfloat dy) -> void
{
    point->move(dx, dy);
    text->move(dx, dy);
}


Graph::CaptionContainer::CaptionContainer(GLfloat width, GLfloat height, glm::vec2 position, GLfloat indent_x, GLfloat indent_y)
    : width(width), height(height), position(position), indent_x(indent_x), indent_y(indent_y),
    current_height(0), last_height(0), borders_enabled(false) {}

auto Graph::CaptionContainer::draw() -> void
{
    for (const auto& captions_vector : captions)
    {
        for (const auto& caption : captions_vector)
        {
            caption->draw();
        }
    }
    if (borders_enabled)
    {
        for (const auto& border : borders)
        {
            border->draw();
        }
    }
}


Graph::Graph(const wchar_t* abscissa, const wchar_t* ordinate, AxisValue hor_zero_value, AxisValue ver_zero_value,
             GLfloat hor_value_step, GLfloat ver_value_step, GLint hor_delims, GLint ver_delims,
             GLint hor_center, GLint ver_center, GLint hor_value_skip, GLint ver_value_skip,
             GLfloat hor_grid_step, GLfloat ver_grid_step, glm::vec2 position)
{
    this->enabled = false;

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
    this->ver_center     = ver_center;

    GLfloat hor_offset = (hor_delims % 2 ? 0 : hor_grid_step / 2);
    GLfloat ver_offset = (ver_delims % 2 ? 0 : ver_grid_step / 2);

    left_border  = -hor_grid_step * (hor_delims / 2) + hor_offset;
    right_border =  hor_grid_step * (hor_delims / 2) - hor_offset;
    up_border    =  ver_grid_step * (ver_delims / 2) - ver_offset;
    down_border  = -ver_grid_step * (ver_delims / 2) + ver_offset;

    // Shift with given position
    left_border  += position.x;
    right_border += position.x;
    up_border    += position.y;
    down_border  += position.y;

    // * Background lines (grid)
    bg_lines_hor = std::vector<std::shared_ptr<Line>>(hor_delims);
    GLfloat tmp_left = left_border;
    for (int i = 0; i < hor_delims; ++i) {
        auto hor_bg_line = std::make_shared<Line>(glm::vec2(tmp_left, down_border),
                                                  glm::vec2(tmp_left, up_border),
                                                  bg_lines_width, bg_lines_color);
        bg_lines_hor[i] = hor_bg_line;
        tmp_left += hor_grid_step;
    }

    bg_lines_ver = std::vector<std::shared_ptr<Line>>(ver_delims);
    GLfloat tmp_down = down_border;
    for (int i = 0; i < ver_delims; ++i) {
        auto ver_bg_line = std::make_shared<Line>(glm::vec2(left_border, tmp_down),
                                                  glm::vec2(right_border, tmp_down),
                                                  bg_lines_width, bg_lines_color);
        bg_lines_ver[i] = ver_bg_line;
        tmp_down += ver_grid_step;
    }

    // * Main lines (axis)
    if (abs(ver_center) > floor(hor_delims / 2))
    {
        LOG_ERROR("Invalid abscissa axis position\n"
                  "{0:} is provided, but the valid range is [-{1:}, {1:}]",
                  ver_center, (GLint) floor(hor_delims / 2));
        std::exit(EXIT_FAILURE);
    }
    if (abs(hor_center) > floor(ver_delims / 2))
    {
        LOG_ERROR("Invalid ordinate axis position\n"
                  "{0:} is provided, but the valid range is [-{1:}, {1:}]",
                  hor_center, (GLint) floor(ver_delims / 2));
        std::exit(EXIT_FAILURE);
    }

    GLfloat hor_axis_offset = hor_center * ver_grid_step - ver_offset;
    GLfloat ver_axis_offset = ver_center * hor_grid_step - hor_offset;

    // Shift with given position
    hor_axis_offset += position.y;
    ver_axis_offset += position.x;

    main_lines = std::vector<std::shared_ptr<Line>>(2);
    auto hor_main_line = std::make_shared<Line>(glm::vec2(left_border, hor_axis_offset),
                                                glm::vec2(right_border, hor_axis_offset),
                                                main_lines_width, axis_color);
    main_lines[0] = hor_main_line;
    auto ver_main_line = std::make_shared<Line>(glm::vec2(ver_axis_offset, down_border),
                                                glm::vec2(ver_axis_offset, up_border),
                                                main_lines_width, axis_color);
    main_lines[1] = ver_main_line;

    // * Arrows
    arrows = std::vector<std::shared_ptr<Triangle>>(2);
    auto hor_arrow = std::make_shared<Triangle>(glm::vec2(right_border + main_lines_width * 2, hor_axis_offset),
                                                glm::vec2(right_border - main_lines_width * 2, hor_axis_offset - main_lines_width * 2),
                                                glm::vec2(right_border - main_lines_width * 2, hor_axis_offset + main_lines_width * 2),
                                                axis_color);
    arrows[0] = hor_arrow;
    auto ver_arrow = std::make_shared<Triangle>(glm::vec2(ver_axis_offset, up_border + main_lines_width * 2),
                                                glm::vec2(ver_axis_offset + main_lines_width * 2, up_border - main_lines_width * 2),
                                                glm::vec2(ver_axis_offset - main_lines_width * 2, up_border - main_lines_width * 2),
                                                axis_color);
    arrows[1] = ver_arrow;

    // * Text
    hor_values = std::vector<AxisValue>(hor_delims);
    ver_values = std::vector<AxisValue>(ver_delims);

    // Abscissa values init
    initialize_axis_texts(hor_values, hor_zero_value, hor_delims, ver_center, hor_value_step);
    // Ordinate values init
    initialize_axis_texts(ver_values, ver_zero_value, ver_delims, hor_center, ver_value_step);

    // Horizontal texts
    hor_texts = std::vector<std::shared_ptr<Text>>(hor_delims);
    for (int i = 0; i < hor_delims; ++i)
    {
        hor_rotation_deg = 55.0f;  // ! Hardcode
        hor_rotation_rad = hor_rotation_deg * M_PI / 180.0f;

        std::wstring raw_text = get_raw_text(hor_values[i]);
        auto text = std::make_shared<Text>(raw_text.c_str(),
                                           glm::vec2(left_border + i * hor_grid_step,
                                                     down_border - ver_grid_step / 4),
                                                     0.4f, hor_rotation_deg);
        text->move(-(text->get_width_ndc() * cos(hor_rotation_rad) - text->get_height_ndc() * sin(hor_rotation_rad)),
                   -(text->get_width_ndc() * sin(hor_rotation_rad) + text->get_height_ndc() * cos(hor_rotation_rad)));
        hor_texts[i] = text;
    }

    // Vertical texts
    ver_texts = std::vector<std::shared_ptr<Text>>(ver_delims);
    for (int i = 0; i < ver_delims; ++i)
    {
        // TODO: Refactor like horizontal text above?
        std::wstring raw_text = get_raw_text(ver_values[i]);
        auto text = std::make_shared<Text>(raw_text.c_str(),
                                           glm::vec2(left_border - hor_grid_step / 2,
                                                     down_border + i * ver_grid_step),
                                                     0.4f, 0.0f);    // ! Hardcode
        text->move(-text->get_width_ndc(), -text->get_height_ndc() / 2);
        ver_texts[i] = text;
    }

    // * Axis labels
    axis_labels = std::vector<std::shared_ptr<Text>>(2);
    auto hor_label = std::make_shared<Text>(abscissa,
                                            glm::vec2(right_border + hor_grid_step / 1.5f,
                                                      hor_axis_offset),
                                                      0.65f, 0.0f);
    hor_label->move(0.0f, -hor_label->get_height_ndc() / 2);
    axis_labels[0] = hor_label;
    auto ver_label = std::make_shared<Text>(ordinate,
                                            glm::vec2(ver_axis_offset,
                                                      up_border + ver_grid_step / 1.5f),
                                                      0.65f, 0.0f);
    ver_label->move(-ver_label->get_width_ndc() / 2, 0.0f);
    axis_labels[1] = ver_label;
}

Graph::~Graph() = default;

auto Graph::draw() -> void
{
    // * Render order to avoid incorrect overlaying:
    // * bg_lines -> main_lines -> arrows -> segments -> points -> text -> labels
    if (enabled)
    {
        for (const auto& bg_line: bg_lines_hor)
        {
            bg_line->draw();
        }
        for (const auto& bg_line: bg_lines_ver)
        {
            bg_line->draw();
        }
        for (const auto& main_line: main_lines)
        {
            main_line->draw();
        }
        for (const auto& arrow: arrows)
        {
            arrow->draw();
        }
        // TODO: add draw() method to Curve class
        for (const auto& [name, curve] : curves)
        {
            curve->draw();
        }
        for (const auto& hor_text: hor_texts)
        {
            hor_text->draw();
        }
        for (const auto& ver_text: ver_texts)
        {
            ver_text->draw();
        }
        for (const auto& label: axis_labels)
        {
            label->draw();
        }
        for (const auto& [name, container] : caption_containers)
        {
            container->draw();
        }
    }
}

auto Graph::create_curve(const wchar_t* name, glm::vec3 color) -> void
{
    if (curves.contains(name))
    {
        LOG_ERROR("Curve with given name already exists in this Graph.");
        std::exit(EXIT_FAILURE);
    }
    curves.emplace(name, std::make_shared<Curve>(color));
}

auto Graph::create_curve(const wchar_t* name, GLfloat R, GLfloat G, GLfloat B) -> void
{
    create_curve(name, glm::vec3(R, G, B));
}

auto Graph::create_caption_container(const wchar_t* name, GLfloat width, GLfloat height, glm::vec2 container_position, GLfloat indent_x, GLfloat indent_y) -> void
{
    auto [it, success] = caption_containers.emplace(name, std::make_shared<CaptionContainer>(width, height, container_position, indent_x, indent_y));
    if (!success)
    {
        LOG_ERROR("Can't create new CaptionContainer. Given name already exists.");
        std::exit(EXIT_FAILURE);
    }

    caption_containers.at(name)->captions.emplace_back();
    caption_containers.at(name)->sizes_width.push_back(0);

    // CaptionContainer borders: left, right, down, up. Mostly for size testing
    GLfloat cx = container_position.x;
    GLfloat cy = container_position.y;
    caption_containers.at(name)->borders.emplace_back(std::make_shared<Line>(cx - width, cy - height - 0.005f,
                                                                             cx - width, cy + height + 0.005f,
                                                                            0.01f, 0.0f, 0.0f, 0.0f));
    caption_containers.at(name)->borders.emplace_back(std::make_shared<Line>(cx + width, cy + height + 0.005f,
                                                                            cx + width, cy - height - 0.005f,
                                                                            0.01f, 0.0f, 0.0f, 0.0f));
    caption_containers.at(name)->borders.emplace_back(std::make_shared<Line>(cx - width - 0.005f, cy - height,
                                                                            cx + width + 0.005f, cy - height,
                                                                            0.01f, 0.0f, 0.0f, 0.0f));
    caption_containers.at(name)->borders.emplace_back(std::make_shared<Line>(cx + width + 0.005f, cy + height,
                                                                            cx - width - 0.005f, cy + height,
                                                                            0.01f, 0.0f, 0.0f, 0.0f));
}

auto Graph::create_caption_container(const wchar_t* name, GLfloat width, GLfloat height, GLfloat x, GLfloat y, GLfloat indent_x, GLfloat indent_y) -> void
{
    create_caption_container(name, width, height, glm::vec2(x, y));
}

auto Graph::create_caption(const wchar_t* container, const wchar_t* curve, GLboolean force_new_line,
                           const wchar_t* name, GLfloat point_size, GLfloat text_size) -> void
{
    // Check if CaptionContainer and Curve exists
    if (!caption_containers.contains(container))
    {
        LOG_ERROR("No CaptionContainer with given name exists.");
        std::exit(EXIT_FAILURE);
    }
    if (!curves.contains(curve))
    {
        LOG_ERROR("No Curve with given name exists.");
        std::exit(EXIT_FAILURE);
    }
    const auto this_container = caption_containers.at(container);
    const auto this_curve = curves.at(curve);

    // Precalculate Text and Caption sizes
    GLfloat* text_params = Text::precalculate_text_size(name ? name : curve, text_size);
    GLfloat text_width_ndc = text_params[2];
    GLfloat text_height_ndc = text_params[3];

    GLfloat caption_width = point_size * 3 + text_width_ndc;
    GLfloat caption_height = std::max(point_size * 2, text_height_ndc);

    // Height check if we need a new line
    GLboolean need_indent_x = !(this_container->captions.end()-1)->empty();
    GLfloat new_width = *(this_container->sizes_width.end()-1) + caption_width + (need_indent_x ? this_container->indent_x : 0);
    GLboolean fits = new_width < this_container->width * 2;

    GLboolean need_indent_y = false;
    if (force_new_line || !fits)
    {
        need_indent_y = true;

        GLfloat new_height = this_container->current_height + this_container->indent_y + text_height_ndc;
        if (new_height >= this_container->height * 2)
        {
            LOG_WARNING("CaptionContainer doesn't have enough height for new Caption.\n"
                        "Required {:.3f}, but only {:.3f} available.",
                        new_height, this_container->height * 2);
            return;
        }

        new_width -= *(this_container->sizes_width.end()-1) + (need_indent_x ? this_container->indent_x : 0);
        need_indent_x = false;

        // Move existing Captions up
        for (const auto& captions_vector : this_container->captions)
        {
            for (const auto& caption: captions_vector)
            {
                caption->move(0.0f, (this_container->indent_y + caption_height) / 2);
            }
        }

        this_container->captions.emplace_back();
        this_container->sizes_width.push_back(0);
        this_container->last_height = this_container->current_height;
    }

    // Width check
    fits = new_width < this_container->width * 2;
    if (!fits)
    {
        LOG_WARNING("CaptionContainer doesn't have enough width for new Caption.\n"
                    "Required {:.3f}, but only {:.3f} available.",
                    new_width, this_container->width * 2);
        return;
    }

    // Move existing Captions left
    for (const auto& caption : *(this_container->captions.end()-1))
    {
        caption->move(-(this_container->indent_x + caption_width) / 2, 0.0f);
    }

    // Create Point, Text and Caption
    const auto point = std::make_shared<Point>(this_container->position, point_size, this_curve->color);
    const auto text = std::make_shared<Text>(name ? name : curve, this_container->position, text_size);
    text->move(-text->get_width_ndc() / 2, -text->get_height_ndc() / 2);

    point->move(-caption_width / 2 + point_size, 0.0f);
    text->move(caption_width / 2 - text->get_width_ndc() / 2, 0.0f);

    const auto caption = std::make_shared<Caption>();
    caption->point = point;
    caption->text = text;

    // Move new Caption to its position
    GLboolean not_first_line_and_pos = this_container->captions.size() > 1 && !(this_container->captions.end() - 1)->empty();
    GLfloat dx = ((need_indent_x ? this_container->indent_x : 0) + *(this_container->sizes_width.end()-1)) / 2;
    GLfloat dy = (need_indent_y || not_first_line_and_pos ? -(this_container->indent_y + this_container->last_height) / 2 : 0);
    caption->move(dx, dy);

    // Updating sizes
    *(this_container->sizes_width.end()-1) += (need_indent_x ? this_container->indent_x : 0) + caption_width;
    if (!need_indent_x)
    {
        this_container->current_height += (need_indent_y ? this_container->indent_y : 0) + caption_height;
    }

    (this_container->captions.end()-1)->push_back(caption);
}

auto Graph::set_container_borders_enabled(const wchar_t* container, bool state) -> void
{
    if (!caption_containers.contains(container))
    {
        LOG_ERROR("No CaptionContainer with given name exists.");
        std::exit(EXIT_FAILURE);
    }
    caption_containers.at(container)->borders_enabled = state;
}

auto Graph::add_point(const wchar_t* curve_name, AxisValue x, AxisValue y) -> void
{
    if (!curves.contains(curve_name))
    {
        LOG_ERROR("No Curve with given name exists.");
        std::exit(EXIT_FAILURE);
    }
    auto this_curve = curves.at(curve_name);

    GLfloat x_part = find_value(0, x);
    GLfloat y_part = find_value(1, y);

    // Now I understand what technical debt is... The fuck these variable names are?
    if (x_part == -1.0f || y_part == -1.0f)
    {
        LOG_WARNING("Can't create a point. No matching coordinate found.");
        return;
    }

    if (x_part > 1.0f)
    {
        if (x_part > 2.0f)
        {
            LOG_WARNING("Can't create a point. X coordinate goes further than 2 horizontal axis'.");
            return;
        }

        GLfloat x_fract = x_part - 1.0f;
        GLint values_to_move = ceil(x_fract * (hor_delims - 1));
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
        for (const auto& [name, curve] : curves)
        {
            for (const auto& point : curve->points)
            {
                point->move(-values_to_move * hor_grid_step, 0.0f);
            }
            for (const auto& segment : curve->segments)
            {
                segment->move(-values_to_move * hor_grid_step, 0.0f);
            }
        }
    }

    GLfloat x_pos = left_border + x_part * (right_border - left_border);
    GLfloat y_pos = down_border + y_part * (up_border - down_border);

    // * Real Point creation is here (I need to split that function...)
    auto point = std::make_shared<Point>(glm::vec2(x_pos, y_pos), 0.0075f, this_curve->color);
    this_curve->points.push_back(point);

    // Connect last two points with a segment
    if (this_curve->points.size() >= 2)
    {
        add_segment(curve_name, (*(this_curve->points.end()-2))->get_position(), (*(this_curve->points.end()-1))->get_position());
    }

    // If some points/segments gone further than left border, delete them
    for (auto it = this_curve->points.begin(); it < this_curve->points.end();)
    {
        if ((*it)->get_position().x < left_border)
        {
            it = this_curve->points.erase(it);
            continue;
        }
        it++;
    }

    for (auto it = this_curve->segments.begin(); it < this_curve->segments.end();)
    {
        if ((*it)->get_end_position().x <= left_border)
        {
            it = this_curve->segments.erase(it);
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

auto Graph::add_segment(const wchar_t* curve_name, glm::vec2 start, glm::vec2 end) -> void
{
    if (!curves.contains(curve_name))
    {
        LOG_ERROR("No Curve with given name exists.");
    }
    auto this_curve = curves.at(curve_name);

    if (start.x > end.x)
    {
        std::swap(start, end);
    }
    auto segment = std::make_shared<Line>(glm::vec2(start.x, start.y), glm::vec2(end.x, end.y), 0.015f, this_curve->color);
    this_curve->segments.push_back(segment);
}

auto Graph::move(glm::vec2 delta) -> void
{
    move(delta.x, delta.y);
}

auto Graph::move(GLfloat dx, GLfloat dy) -> void
{
    left_border  += dx;
    right_border += dx;
    up_border    += dy;
    down_border  += dy;

    for (const auto& bg_line : bg_lines_hor)
    {
        bg_line->move(dx, dy);
    }
    for (const auto& bg_line : bg_lines_ver)
    {
        bg_line->move(dx, dy);
    }
    for (const auto& main_line : main_lines)
    {
        main_line->move(dx, dy);
    }
    for (const auto& arrow: arrows) {
        arrow->move(dx, dy);
    }
    for (const auto& [name, curve] : curves)
    {
        for (const auto& segment : curve->segments) {
            segment->move(dx, dy);
        }
        for (const auto& point : curve->points) {
            point->move(dx, dy);
        }
    }
    for (const auto& hor_text: hor_texts) {
        hor_text->move(dx, dy);
    }
    for (const auto& ver_text: ver_texts) {
        ver_text->move(dx, dy);
    }
    for (const auto& label: axis_labels) {
        label->move(dx, dy);
    }
}

auto Graph::set_position(glm::vec2 new_position) -> void
{
    glm::vec2 delta = new_position - position;
    move(delta);
}

auto Graph::set_position(GLfloat new_x, GLfloat new_y) -> void
{
    set_position(glm::vec2(new_x, new_y));
}


auto Graph::set_enabled(bool state) -> void
{
    enabled = state;
}

auto Graph::enable() -> void
{
    enabled = true;
}

auto Graph::disable() -> void
{
    enabled = false;
}

auto Graph::is_enabled() const -> bool
{
    return enabled;
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
                           values[j] = alternative + i * std::chrono::milliseconds(static_cast<GLint64>(value_step));
                       }
                   }
               }, zero_value);
}

auto Graph::get_raw_text(AxisValue variant) -> std::wstring
{
    return std::visit([](auto&& alternative) -> std::wstring
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
                return to_wstring(str);
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
            LOG_ERROR("Incorrect axis provided");
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
                    GLint64 offset    = std::chrono::duration_cast<std::chrono::milliseconds>(get<TimePoint>(*(values->begin())).time_since_epoch()).count();
                    GLint64 end_value = std::chrono::duration_cast<std::chrono::milliseconds>(get<TimePoint>(*(values->end()-1)).time_since_epoch()).count();
                    GLint64 value     = std::chrono::duration_cast<std::chrono::milliseconds>(alternative.time_since_epoch()).count();
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
                    result = alternative + std::chrono::milliseconds(static_cast<GLint64>(step));
                }
                return result;
            }, current);
}

auto Graph::update_labels() -> void
{
    for (int i = 0; i < hor_delims; ++i)
    {
        std::wstring raw_text = get_raw_text(hor_values[i]);
        hor_texts[i]->set_text(raw_text.c_str());
        hor_texts[i]->set_position(glm::vec2(left_border + i * hor_grid_step,
                                             down_border - ver_grid_step / 4));
        hor_texts[i]->move(-(hor_texts[i]->get_width_ndc() * cos(hor_rotation_rad) - hor_texts[i]->get_height_ndc() * sin(hor_rotation_rad)),
                           -(hor_texts[i]->get_width_ndc() * sin(hor_rotation_rad) + hor_texts[i]->get_height_ndc() * cos(hor_rotation_rad)));
        //hor_texts[i]->move(position);
    }
}

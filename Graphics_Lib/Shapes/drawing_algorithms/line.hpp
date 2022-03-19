#ifndef DRAW_LINE_HPP
#define DRAW_LINE_HPP

#include <vector>
#include "../../V2d.hpp"
#include "../../frame.hpp"

void compute_line_stroke(Frame & frame, int fx, int fy, int tx, int ty, const Rgba & color, int sw);
void compute_stroke_part_horisontal(Frame & frame, int x, int y, const Rgba & color, int sw);
void compute_stroke_part_vertical(Frame & frame, int x, int y, const Rgba & color, int sw);

#endif
#ifndef DRAW_LINE_HPP
#define DRAW_LINE_HPP

#include <vector>
#include "../../V2d.hpp"
#include "../../frame.hpp"

void compute_line_stroke(Frame & frame, const V2d<int> & f, const V2d<int> & t, const Rgba & color);

#endif
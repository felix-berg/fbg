#ifndef DRAW_LINE_HPP
#define DRAW_LINE_HPP

#include <vector>
#include "../../V2d.hpp"
#include "../../rgba.hpp"

void compute_line_stroke(Rgba * pixels, const V2d<int> & f, const V2d<int> & t, const Rgba & color, int scr_w, int scr_h);

#endif
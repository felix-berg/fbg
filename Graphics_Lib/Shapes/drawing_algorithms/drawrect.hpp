#ifndef DRAW_RECT_HPP
#define DRAW_RECT_HPP

#include "../../rgba.hpp"
#include "../../frame.hpp"

namespace fbg {
void compute_AA_rect_stroke(Frame<Rgba> & f, int x, int y, int w, int h, const Rgba & color, int sw);
void compute_AA_rect_fill(Frame<Rgba> & f, int x, int y, int w, int h, const Rgba & color);
};
#endif
#ifndef DRAW_CIRCLE_HPP
#define DRAW_CIRCLE_HPP

#include "../circle.hpp"
#include "../../frame.hpp"

void compute_circle_stroke(Frame & f, int x, int y, int r, const Rgba & color, int sw);
void compute_circle_fill(Frame & f, int x, int y, int r, const Rgba & color);

#endif
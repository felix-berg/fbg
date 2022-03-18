#ifndef DRAW_RECT_HPP
#define DRAW_RECT_HPP

#include "../../frame.hpp"
#include "../rect.hpp"

void compute_AA_rect_stroke(Frame & f, Rectangle & r);
void compute_AA_rect_fill(Frame & f, Rectangle & r);

#endif
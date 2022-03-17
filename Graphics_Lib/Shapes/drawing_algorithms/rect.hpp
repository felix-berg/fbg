#ifndef DRAW_RECT_HPP
#define DRAW_RECT_HPP

#include "../../rgba.hpp"
#include "../rect.hpp"

void compute_AA_rect_stroke(Rgba * pixels, Rectangle & r, int scr_w, int scr_h);

#endif
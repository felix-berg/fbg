#ifndef DRAW_CIRCLE_HPP
#define DRAW_CIRCLE_HPP

#include "../../frame.hpp"

// this file is only defined in headers, because it uses templates.

namespace fbg
{
   void compute_circle_stroke(Frame & frame, int cx, int cy, int r, Rgba color, int sw);
   void compute_circle_fill(Frame & frame, int cx, int cy, int r, Rgba color);
};
#endif
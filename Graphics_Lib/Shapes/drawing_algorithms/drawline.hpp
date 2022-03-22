#ifndef DRAW_LINE_HPP
#define DRAW_LINE_HPP

#include "../../rgba.hpp"
#include "../../frame.hpp"

namespace fbg {
   void compute_line_stroke(Frame & frame, int fx, int fy, int tx, int ty, const Rgba & color, int sw);
   void compute_line_stroke_smooth(Frame & frame, int fx, int fy, int tx, int ty, const Rgba & color, int sw);
};
#endif
#ifndef DRAW_LINE_HPP
#define DRAW_LINE_HPP

#include "../../rgba.hpp"
#include "../../frame.hpp"

namespace fbg {
   void compute_line_stroke(Frame<Rgba> & frame, int fx, int fy, int tx, int ty, const Rgba & color, int sw);
   void compute_line_stroke_smooth(Frame<Rgba> & frame, int fx, int fy, int tx, int ty, const Rgba & color, int sw);
   void draw_stroke_part_horisontal(Frame<Rgba> & frame, int x, int y, const Rgba & color, int sw);
   void draw_stroke_part_vertical(Frame<Rgba> & frame, int x, int y, const Rgba & color, int sw);
};
#endif
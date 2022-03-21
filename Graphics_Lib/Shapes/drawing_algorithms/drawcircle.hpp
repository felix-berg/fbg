#ifndef DRAW_CIRCLE_HPP
#define DRAW_CIRCLE_HPP

#include "../../frame.hpp"

namespace fbg {
   // template <typename Col>
   void compute_circle_stroke(Frame<Rgba> & f, int x, int y, int r, const Rgba & color, int sw);
   
   // template <typename Col>
   void compute_circle_fill(Frame<Rgba> & f, int x, int y, int r, const Rgba & color);
};
#endif
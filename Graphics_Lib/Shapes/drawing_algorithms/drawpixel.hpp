#ifndef PIXEL_HPP
#define PIXEL_HPP

#include "../../rgba.hpp"
#include "../../frame.hpp"

namespace fbg {
   void set_horisontal_line(Frame & frame, int fx, int tx, int y, const Rgba & color);
   void draw_pixel(Frame & frame, int x, int y, const Rgba & color, int sw);
   void offsets_from_strokeweight(int sw, int * lOff, int * rOff);
};

#endif
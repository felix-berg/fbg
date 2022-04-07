#ifndef PIXEL_HPP
#define PIXEL_HPP

#include "../../rgba.hpp"
#include "../../frame.hpp"

namespace fbg {
   void set_horisontal_line(Frame & frame, int fx, int tx, int y, Rgba color);
   void set_vertical_line(Frame & frame, int x, int fy, int ty, Rgba color);
   void draw_pixel(Frame & frame, int x, int y, Rgba color, int sw);
   void offsets_from_strokeweight(int sw, int * lOff, int * rOff);
};

#endif
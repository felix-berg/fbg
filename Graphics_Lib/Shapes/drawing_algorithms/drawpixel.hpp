#ifndef PIXEL_HPP
#define PIXEL_HPP

#include "../../rgba.hpp"
#include "../../frame.hpp"

namespace fbg {
template <typename T> bool in_bound(T x, T y, T lx, T ly, T ux, T uy);
void set_pixel(Frame<Rgba> & frame, int x, int y, const Rgba & color);
void set_pixel(Frame<Rgba> & frame, int x, int y, const Rgba & color, int sw);
void compute_horisontal_line(Frame<Rgba> & frame, int fx, int fy, int tx, int ty, const Rgba & color);
void offsets_from_strokeweight(int sw, int * lOff, int * rOff);
};
#endif
#ifndef PIXEL_HPP
#define PIXEL_HPP

#include "../../frame.hpp"
#include "../../rgba.hpp"

template <typename T> bool in_bound(T x, T y, T lx, T ly, T ux, T uy);
void set_pixel(Frame & frame, int x, int y, const Rgba & color);

#endif
#ifndef PIXEL_HPP
#define PIXEL_HPP

#include "../../frame.hpp"
#include "../../rgba.hpp"
#include "../../V2d.hpp"

template <typename T> bool in_bound(T x, T y, T lx, T ly, T ux, T uy);
void set_pixel(Frame & frame, int x, int y, const Rgba & color);
void compute_horisontal_line(Frame & frame, V2d<int> f, V2d<int> t, const Rgba & color);
void restrict_pxi_ll_from_point(const V2d<int> & pt, int * pixel_idx, int * line_length, int scr_w, int scr_h);

#endif
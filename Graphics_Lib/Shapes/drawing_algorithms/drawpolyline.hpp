#ifndef DRAW_POLYGON_HPP
#define DRAW_POLYGON_HPP

#include "../../frame.hpp"
#include "../../V2d.hpp"

#include <vector>

namespace fbg {
   void compute_polyline_convex_fill(Frame & f, std::vector<V2d<float>> lst, const Rgba & color);
};

#endif
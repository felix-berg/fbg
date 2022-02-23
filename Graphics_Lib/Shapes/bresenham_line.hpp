#ifndef BRESENHAM_LINE_HPP
#define BRESENHAM_LINE_HPP

#include <vector>
#include "../V2d.hpp"

std::vector<V2d<int>> get_points_for_line(const V2d<int> & f, const V2d<int> & t);

#endif
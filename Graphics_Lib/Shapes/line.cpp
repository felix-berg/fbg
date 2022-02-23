#include "line.hpp"
#include "../V2d.hpp"
#include "../rgba.hpp"
#include "bresenham_line.hpp"

#include <iostream>

Line::Line(const V2d<int> & f, const V2d<int> & t) 
	: Shape {{ f, t }}
{}


std::vector<V2d<int>> Line::compute_lines() {
	std::vector<V2d<int>> points;
	points = get_points_for_line(get_point(0), get_point(1));
	return points;
}
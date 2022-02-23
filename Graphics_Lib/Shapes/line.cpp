#include "line.hpp"
#include "../V2d.hpp"
#include "../rgba.hpp"
#include "shape_algorithms.hpp"

#include <iostream>
#include <cmath>

Line::Line(const V2d<int> & f, const V2d<int> & t) 
	: Shape {{ f, t }}
{}


void Line::compute_lines(Rgba * pixels, int width, int height) {
	std::vector<V2d<int>> points;

	points = get_points_for_line(get_point(0), get_point(1));

	for (V2d<int> & p : points) {
		if (p.is_bound({0, 0}, {width - 1, height - 1}))
			pixels[p.x + p.y * width].combine_with(get_stroke());
	}
}
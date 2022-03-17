#include "line.hpp"
#include "../V2d.hpp"
#include "../rgba.hpp"
#include "drawing_algorithms/line.hpp"
#include "../alphacomposite.hpp"

#include <iostream>
#include <cmath>

Line::Line(const V2d<int> & f, const V2d<int> & t) 
	: Shape {{ f, t }}
{}

/*
	Change given pixelbuffer based on the pixels of the line.
*/
void Line::compute_stroke(Rgba * pixels, int width, int height) {
	std::vector<V2d<int>> points;

	points = get_points_for_line(get_point(0), get_point(1));

	for (const V2d<int> & p : points)
		if (p.is_bound({0, 0}, {width - 1, height - 1})) {
			alpha_composite1(pixels + p.x + p.y * width, &stroke());
		}
}
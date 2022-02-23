#include <vector>
#include "../V2d.hpp"
#include "bresenham_line.hpp"

std::vector<V2d<int>> got_points_for_line_low(const V2d<int> & f, const V2d<int> & t);
std::vector<V2d<int>> get_points_for_line_high(const V2d<int> & f, const V2d<int> & t);

/*
	Returns a vector of points for the pixels of the line given by points t and f.
*/
std::vector<V2d<int>> get_points_for_line(const V2d<int> & f, const V2d<int> & t) {
	if (abs(t.x - f.x) > abs(t.y - f.y)) {
		if (f.x > t.x) // if points are the opposite way round
			return got_points_for_line_low(t, f);
		else 
			return got_points_for_line_low(f, t);
	} else 
		if (f.y > t.y)
			return get_points_for_line_high(t, f);
		else
			return get_points_for_line_high(f, t);
}


/*
	Bresenhams line algorithm: https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm
	Case for when absolute change in x is greater than absolute change in y. abs(gradient) < 1.
	Stepping along x-axis -> only one point per column
*/
std::vector<V2d<int>> got_points_for_line_low(const V2d<int> & f, const V2d<int> & t) {
	std::vector<V2d<int>> points; 

	int dx = t.x - f.x;
	int dy = t.y - f.y;

	if (dy <= points.max_size()) points.reserve(dy);

	int y_dir = 1;
	if (dy < 0) {
		y_dir = -1;
		dy *= -1;
	}
	
	// calculate the first error absolutely -> rest are calculated accumulatively
	int D  = 2 * dy - dx;
	int y  = f.y;

	// loop along x-axis: from.x -> to.y
	for (int x = f.x; x <= t.x; x++) {
		points.push_back({x, y});
		if (D > 0) { // the point is under the line, the error is positive. Step one pixel down (or up if y_dir is negative)	
			y += y_dir;
			D -= 2 * dx;
		}
		D += 2 * dy;
	}

	// move the created array
	return std::move(points);
}

/*
	Bresenhams line algorithm: https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm
	Case for when change in y is greater than change in x. the absolute gradient is bigger than 1
	Stepping along y-axis -> only one point per row.
	Every formula is switched from the normal "low" version
*/
std::vector<V2d<int>> get_points_for_line_high(const V2d<int> & f, const V2d<int> & t) {
	std::vector<V2d<int>> points; 

	int dx = t.x - f.x;
	int dy = t.y - f.y;

	if (dx <= points.max_size()) points.reserve(dx);

	int x_dir = 1;
	if (dx < 0) {
		x_dir = -1;
		dx *= -1;
	}
	
	int D  = 2 * dx - dy;

	int x = f.x;

	for (int y = f.y; y <= t.y; y++) {
		points.push_back({x, y});
		if (D > 0) {
			x += x_dir;
			D -= 2 * dy;
		}
		D += 2 * dx;
	}

	// move the created array
	return std::move(points);
}



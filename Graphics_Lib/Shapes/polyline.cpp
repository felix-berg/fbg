#include "polyline.hpp"
#include "drawing_algorithms/line.hpp"

void fbg::Polygon::draw_stroke(Frame & frame) {
	// Set from to the last vector
	V2d<int> to, from = get_point(0);

	// draw every line segment by looping through list of points
	for (int i = 0; i < num_points(); i++) {
		to = get_point(i);
		compute_line_stroke(frame, from.x, from.y, to.x, to.y, stroke(), strokeweight());
		from = to;
	}

	// if polyline is closed, draw the remaining line.
	if (!m_open) {
		from = get_point(num_points() - 1);
		to = get_point(0);
		compute_line_stroke(frame, from.x, from.y, to.x, to.y, stroke(), strokeweight());
	}
}
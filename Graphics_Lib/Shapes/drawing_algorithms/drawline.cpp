#include "drawline.hpp"
#include "drawpixel.hpp"
#include "drawcircle.hpp"
#include "bresenham.hpp"

#include <vector>

using namespace fbg;

void draw_stroke_low(Frame & frame, int fx, int fy, int tx, int ty, const Rgba & color, int sw, bool smoothEdges); 
void draw_stroke_hi(Frame & frame, int fx, int fy, int tx, int ty, const Rgba & color, int sw, bool smoothEdges);

/*
	Compute the colors of the lines stroke and output onto "frame".
	Line defined by fx, fy -> tx, ty (inclusive).
	This function doesn't make smooth edges.
*/
void fbg::compute_line_stroke(Frame & frame, int fx, int fy, int tx, int ty, const Rgba & color, int sw) {
	if (abs(tx - fx) > abs(ty - fy)) { // if gradient < 1
		if (fx > tx) // if points are the opposite way round
			draw_stroke_low(frame, tx, ty, fx, fy, color, sw, false);
		else 
			draw_stroke_low(frame, fx, fy, tx, ty, color, sw, false);
	} else // if gradient > 1, use "hi"
		if (fy > ty)
			draw_stroke_hi(frame, tx, ty, fx, fy, color, sw, false);
		else
			draw_stroke_hi(frame, fx, fy, tx, ty, color, sw, false);
}

/*
	Compute the vertical pixels for a given pixel with the correct stroke
*/
void fbg::draw_stroke_part_vertical(Frame & frame, int x, int y, const Rgba & color, int sw) {
	int lOff, rOff;
	offsets_from_strokeweight(sw, &lOff, &rOff);

	int fy = y - lOff;
	int ty = y + rOff;

	for (int py = fy; py <= ty; py++)
		frame.set_pixel(x, py, color);
}

/*
	Compute the horisontal pixels for a given pixel with the correct stroke
*/
void fbg::draw_stroke_part_horisontal(Frame & frame, int x, int y, const Rgba & color, int sw) {
	int lOff, rOff;
	offsets_from_strokeweight(sw, &lOff, &rOff);
	int fx = x - lOff; 
	int tx = x + rOff; // from x and to x
	
	set_horisontal_line(frame, fx, tx, y, color);
}

/** Get coordinates for point of perpendicular line within grid.
 * Translates from (x, y) into (&px, &py).  */
void get_perpendicular_line_point(int x, int y, int * px, int * py, int gridWidth, int gradientSign) {
	bool isNegative = gradientSign < 0;

	int lx = isNegative * (gridWidth - 1) + gradientSign * x;
	int ly = y * gradientSign;

	*px = ly;
	*py = gridWidth - 1 - lx;
}

/** Make sure, that min and max are in the right order */
inline void fix_min_max(int * min, int * max) {
	if (*min > *max) { 
		int tmp = *min;
		*min = *max;
		*max = tmp;
	}
}

std::vector<bool> g_perpLineGrid; // container for current grid
int g_perpLineGridPitch = 0; // the current pitch of the g_perpLineGrid

/* Get grid filled with 1's where the perpendicular line to the line defined by (fx, tx) is. */
std::vector<bool> & get_stroke_pattern(int fx, int fy, int tx, int ty, int gridDim) {
	g_perpLineGrid.resize(gridDim * gridDim); // set the size of the container to relevant size
	g_perpLineGridPitch = gridDim;

	std::fill(g_perpLineGrid.begin(), g_perpLineGrid.end(), 0);

	int gradientSign = 2 * (ty - fy >= 0) - 1;

	int cx = gridDim / 2;
	int cy = cx;
	int midx;

	bresenham_line(0, 0, tx - fx, ty - fy, [&](int x, int y) -> bool {
		if (x >= gridDim) return false;

		int px, py;
		get_perpendicular_line_point(x, y, &px, &py, gridDim, gradientSign);

		g_perpLineGrid[py * gridDim + px] = 1;

		if (py == cy)
			midx = px;

		return true; // continue algorithm
	});

	// move every bit of the perpendicular line grid, 
	// so that the middle pixel of the line is in the center
	int perpLineOffset = cx - midx; // the amount to offset every pixel

	int minX, maxX;
	for (int y = 0; y < gridDim; y++)
		for (int x = 0; x < gridDim; x++) {
			int idx = x + y * gridDim;
			if (g_perpLineGrid[idx]) {
				g_perpLineGrid[idx] = 0;
				g_perpLineGrid[idx + perpLineOffset] = 1;

				if (y == 0) 			 minX = x;
				if (y == gridDim - 1) maxX = x;
				break;
			}
		}

	// sort min and max values
	fix_min_max(&minX, &maxX);

	// calculate gradient of line
	int lineX = maxX - minX;
	float gradient = static_cast<float> (gridDim) / static_cast<float> (lineX);

	// compute appropriate grid height based on the line reach X
	int lineY = static_cast<int> (std::sin(std::atan(gradient)) * gridDim); // sin(atan(gradient)) * strokeweight

	// remove top and bottom lines according to gradient
	int n_linesToRemove = gridDim - lineY;
	for (int y = 0; y < n_linesToRemove / 2; y++) {
		// top line
		std::fill_n(g_perpLineGrid.begin() + y * gridDim, gridDim, 0);
		// bottom line
		std::fill_n(g_perpLineGrid.begin() + (gridDim - y) * gridDim, gridDim, 0);
	}

	return g_perpLineGrid;
}

void draw_pixels_after_bool_array(Frame & frame, const std::vector<bool> & grid, int cx, int cy, const Rgba & color) {
	for (int j = 0; j < g_perpLineGridPitch; j++)
		for (int i = 0; i < g_perpLineGridPitch; i++) {
			if (grid[j * g_perpLineGridPitch + i])
				frame.set_pixel(cx + i - g_perpLineGridPitch / 2, cy + j - g_perpLineGridPitch / 2, color);
		}
}

std::vector<std::pair<int, int>> get_gap_points_from_stroke_pattern(const std::vector<bool> & strokePattern, int gridDim, int gradientSign){
	std::vector<std::pair<int, int>> res;



	// compare strokePattern to itself, but shifted -1 x and +1 y.
	for (int y = 0; y < gridDim; y++) 
		for (int x = 0; x < gridDim; x++) {
			int idx = y * gridDim + x;
			if (!strokePattern[idx]) continue;

			int shiftedX, shiftedY;

			if (gradientSign == 1) {
				shiftedX = x - 1;
				shiftedY = y - 1;
			} else {
				shiftedX = x - 1;
				shiftedY = y + 1;
			}
			
			// bounds check
			if (shiftedX < 0 || shiftedX >= gridDim ||
				 shiftedY < 0 || shiftedY >= gridDim)
				break;
				
			int shiftedIndex = shiftedX + shiftedY * gridDim;

			// point was found does it have a gap followed by a pixel?
			// |X|O|X|
			if (strokePattern[idx] && !strokePattern[shiftedIndex + 1] && strokePattern[shiftedIndex + 2])
				res.push_back(std::make_pair(x + 1, y));

			break;
		}

	return std::move(res);
}

void draw_stroke_low(Frame & frame, int fx, int fy, int tx, int ty, const Rgba & color, int sw, bool smoothEdges) {
	int gridDim = sw;

	// reminder that this is a pointer to a global resource
	std::vector<bool> & strokePattern = get_stroke_pattern(fx, fy, tx, ty, gridDim);

	// get the gap-points for the given stroke pattern
	int gradientSign = (ty - fy >= 0) * 2 - 1;
	std::vector<std::pair<int, int>> gapPoints = get_gap_points_from_stroke_pattern(strokePattern, gridDim, gradientSign);
	
	// add gaps to stroke pattern
	for (auto & p : gapPoints)
		strokePattern[p.first + p.second * gridDim] = 1;

	int gridCenterX = gridDim / 2;
	int gridCenterY = gridDim / 2;

	std::vector<bool> circleGrid;
	circleGrid.resize(gridDim * gridDim);

	// lOff -> if lOff != rOff, make circle smaller.
	bresenham_circle(gridDim / 2, [&](int x, int y) -> void {
		// octant 2 -> octant 1
		int idx1 = (gridCenterX + x) + (gridCenterY + y) * gridDim;
		int idx2 = (gridCenterX - x) + (gridCenterY + y) * gridDim;
		std::fill(circleGrid.begin() + idx2, circleGrid.begin() + idx1, 1);

		// octant 4 -> octant 3
		int idx3 = (gridCenterX + x) + (gridCenterY - y) * gridDim;
		int idx4 = (gridCenterX - x) + (gridCenterY - y) * gridDim;
		std::fill(circleGrid.begin() + idx4, circleGrid.begin() + idx3, 1);

		// octant 6 -> 5
		int idx5 = (gridCenterX + y) + (gridCenterY + x) * gridDim;
		int idx6 = (gridCenterX - y) + (gridCenterY + x) * gridDim;
		std::fill(circleGrid.begin() + idx6, circleGrid.begin() + idx5, 1);

		// octant 8 -> 7
		int idx7 = (gridCenterX + y) + (gridCenterY - x) * gridDim;
		int idx8 = (gridCenterX - y) + (gridCenterY - x) * gridDim;
		std::fill(circleGrid.begin() + idx8, circleGrid.begin() + idx7, 1);
	});

	for (int j = 0; j < gridDim; j++) {
		bool lineFound = false; // w
		for (int i = 0; i < gridDim; i++) {
			int idx = i + j * gridDim;

			if (strokePattern[idx]) lineFound = true;
			if (!lineFound) continue;

			// We are past the line -> fill in pixels with circle
			strokePattern[idx] = circleGrid[idx];
		}
	}

	bresenham_line(fx, fy, tx, ty, [&](int x, int y) -> bool {
		draw_pixels_after_bool_array(frame, strokePattern, x, y, color);
		return true;
	});



	// for (int j = 0; j < gridDim; j++) {
	// 	for (int i = 0; i < gridDim; i++)
	// 		std::cout << perpLineGrid[j * gridDim + i] << ' ';
	// 	std::cout << '\n';
	// }
	// std::cout << '\n';
}


void draw_stroke_hi(Frame & frame, int fx, int fy, int tx, int ty, const Rgba & color, int sw, bool smoothEdges) {
	bresenham_line(fy, fx, ty, tx, [&](int x, int y) -> bool {
		// the same as low stroke, but with opposite parameters
		draw_stroke_part_horisontal(frame, y, x, color, sw);
		return true;
	});
}



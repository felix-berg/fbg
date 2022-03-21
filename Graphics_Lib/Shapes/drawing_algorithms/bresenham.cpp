#include "bresenham.hpp"

/** Bresenham line algorithm for drawing a line where the gradient is less than one. 
    Switch y with x to get the high version of the algorithm. */ 
void fbg::bresenham_line(int fx, int fy, int tx, int ty, std::function<void(int, int)> draw_pixel) {
	int dx = tx - fx;
	int dy = ty - fy;

	int yDir = 1;
	if (dy < 0) {
		yDir = -1;
		dy *= -1;
	}

	// calculate the first error absolutely -> rest are calculated accumulatively
	int D  = 2 * dy - dx;
	int y  = fy;

	// loop along x-axis: from.x -> to.y
	for (int x = fx; x <= tx; x++) {
		draw_pixel(x, y);

		if (D > 0) { // the point is under the line, the error is positive. Step one pixel vertically
			y += yDir;
			D -= 2 * dx;
		}
		// no vertical step
		D += 2 * dy;
	}
}

/** This function draws single pixels in the first quadrant, and expects that draw_pixel will handle the other seven. */ 
void fbg::bresenham_circle(int r, std::function<void(int, int)> draw_pixel) {
   int x = r;
	int y = 0;

	int D = 3 - 2 * r;

	while (x >= y) {
		draw_pixel(x, y);

		if (D > 0) {
			D = D + 4 * (y - x) + 10;
			x = x - 1;
		}
		else // D <= 0:
			D = D + 4 * y + 6;

		y = y + 1;
	}
}
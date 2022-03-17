#include "circle.hpp"
#include "../../alphacomposite.hpp"

template <typename T>
bool in_bound(T x, T y, T lx, T ly, T ux, T uy) {
	return ! (x < lx || x > ux ||
				 y < ly || y > uy);
}

void set_pixel(Rgba * pixels, int x, int y, const Rgba & color, int scr_w, int scr_h) {
	if (in_bound(x, y, 0, 0, scr_w, scr_h))
		alpha_composite1(&pixels[y * scr_w + x], &color);
}

void draw_8_octants(Rgba * pixels, int cx, int cy, int x, int y, const Rgba & color, int scr_w, int scr_h) {
	set_pixel(pixels, cx + x, cy + y, color, scr_w, scr_h);
	set_pixel(pixels, cx - x, cy + y, color, scr_w, scr_h);
	set_pixel(pixels, cx + x, cy - y, color, scr_w, scr_h);
	set_pixel(pixels, cx - x, cy - y, color, scr_w, scr_h);
	set_pixel(pixels, cx + y, cy + x, color, scr_w, scr_h);
	set_pixel(pixels, cx - y, cy + x, color, scr_w, scr_h);
	set_pixel(pixels, cx + y, cy - x, color, scr_w, scr_h);
	set_pixel(pixels, cx - y, cy - x, color, scr_w, scr_h);
}

void compute_circle_stroke(Rgba * pixels, Circle & c, int scr_w, int scr_h) {
	int x = c.radius();
	int y = 0;

	int cx = c.pos().x;
	int cy = c.pos().y;

	int D = 3 - 2 * c.radius();

	while (x > y) {
		draw_8_octants(pixels, cx, cy, x, y, c.stroke(), scr_w, scr_h);
		if (D > 0) {
			D = D + 4 * (y - x) + 10;
			x = x - 1;
		}
		else // D <= 0: 
			D = D + 4 * y + 6;
		
		y = y + 1;
	}
}
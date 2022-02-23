#include "Window.hpp"

#define KEY_DELAY 10

void Window::wait_for_key() {
	while (m_keys_down.size() == 0) {
		SDL_Event e;
		SDL_WaitEvent(&e);
		handle_event(&e);
	}
}

void Window::wait_for_key(int key_id) {
	while (!is_key_pressed(key_id)) {
		SDL_Event e;
		SDL_WaitEvent(&e);
		handle_event(&e);
	}
}

void Window::draw() {
	clear_pixels(m_background_color);
	context.compute_lines(m_pixels, width(), height());

	// update the SDL Pixelbuffer and pull new events.
	this->update();
}

// for (int yoff = - stroke_weight / 2; yoff <= stroke_weight / 2; yoff++)
// 	for (int xoff = - stroke_weight / 2; xoff <= stroke_weight / 2; xoff++) {
// 		V2d<int> offset = {xoff, yoff};
// 		V2d<int> draw_point = p + offset;
// 		if (is_bounded(draw_point, {0, 0}, {width() - 1, height() - 1}) && draw_point.x * draw_point.x + draw_point.y * draw_point.y < (stroke_weight / 2) * (stroke_weight / 2));
// 			this->set_pixel(draw_point, shape->get_stroke());
// }
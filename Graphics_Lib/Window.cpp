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
	for (Shape * shape : this->m_shapes) {
		std::vector<V2d<int>> points = shape->compute_lines();
		for (auto & p : points)
			this->set_pixel(p, shape->get_stroke());
	}

	this->update();
}
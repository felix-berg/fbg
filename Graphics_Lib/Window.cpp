#include "Window.hpp"
#include "alphacomposit.hpp"

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
	for (int i = 0; i < size(); i++) {
		alpha_composite1(m_pixels + i, &m_background_color);
	}

	// alpha_compositeNC(m_pixels, &m_background_color, size());
	context.compute_lines(m_pixels, width(), height());

	// update the SDL Pixelbuffer and pull new events.
	this->update();
}
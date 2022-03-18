#include "Window.hpp"
#include "alphacomposite.hpp"

#define KEY_DELAY 10

/*
	Stop the current process, until user enters any key.
*/
void Window::wait_for_key() {
	while (m_keys_down.size() == 0 || !this->has_keyboard_focus()) {
		SDL_Event e;
		SDL_WaitEvent(&e);
		handle_event(&e);
	}
}

void Window::wait_for_key(int key_id) {
	while (!is_key_pressed(key_id) || !this->has_keyboard_focus()) {
		SDL_Event e;
		SDL_WaitEvent(&e);
		handle_event(&e);
	}
}

void Window::draw() {
	// Alpha composite entire pixel area using AVX2 vector processing 
	alpha_compositeNC(frame.pixels, &m_background_color, size());
	// change m_pixels by adding the colors of the strokes within
	// the shapes of the context object
	context.compute_stroke(frame);

	context.compute_fill(frame);
	
	// update the SDL Pixelbuffer and pull new events.
	this->update();
}
#ifndef WINDOW_HPP
#define WINDOW_HPP

#include "Shapes/shape.hpp"
#include "Shapes/context.hpp"
#include "SDL_Handler.hpp"

#include <vector>

class Window : public SDL_Handler {
public:
	Window() : SDL_Handler { } { };
	Window(const std::string & title, unsigned int w, unsigned int h) : SDL_Handler { title, w, h } { };

	void wait_for_key(int key_id);
	void wait_for_key();

	void background(unsigned char brightness) { m_background_color = {brightness, brightness, brightness, brightness}; };
	void background(unsigned char brightness, unsigned char alpha) { m_background_color = {brightness, brightness, brightness, alpha}; };
	void background(const Rgba & color) { m_background_color = color; };
	Rgba get_background_color() const { return m_background_color; };

	void draw();

	Context context;

private:
	std::vector<Shape *> m_shapes;
	Rgba m_background_color = {0, 0, 0, 255};
};

#endif
#ifndef WINDOW_HPP
#define WINDOW_HPP

#include "Shapes/shape.hpp"
#include "Shapes/context.hpp"
#include "SDL_Handler.hpp"

#include <vector>
#include "multithr_alphacomposite.hpp"


class Window : public SDL_Handler {
public:
	Window() {
		init_compositor_threadpool(size());
	};
	Window(const std::string & title, unsigned int w, unsigned int h) : SDL_Handler { title, w, h } {
		init_compositor_threadpool(size());
	};

	void wait_for_key(int key_id);
	void wait_for_key();

	// setters
	void background(u_char brightness) 
		{ m_background_color = {brightness, brightness, brightness, 255}; };

	void background(u_char brightness, u_char alpha) 
		{ m_background_color = {brightness, brightness, brightness, alpha}; };

	void background(u_char r, u_char g, u_char b) 
		{ m_background_color = { r, g, b, 255 }; };
		
	void background(const Rgba & color) 
		{ m_background_color = color; };

	
	// getters
	Rgba background() const { return m_background_color; };

	void draw();

	Context context;

private:
	std::vector<Shape *> m_shapes;
	Rgba m_background_color = {0, 0, 0, 255};
};

#endif
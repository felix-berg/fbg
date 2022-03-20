#ifndef WINDOW_HPP
#define WINDOW_HPP

#include "Shapes/shape.hpp"
#include "Shapes/context.hpp"
#include "SDL_Handler.hpp"

#include <vector>

namespace fbg {
class Window : public SDL_Handler {
public:
	Window() {	};
	Window(const std::string & title, int w, int h) : SDL_Handler { title, w, h } {	};

	void wait_for_key(int key_id);
	void wait_for_key();

	// setters
	void background(u_char brightness) 
		{ m_backgroundColor = {brightness, brightness, brightness, 255}; };

	void background(u_char brightness, u_char alpha) 
		{ 
			m_backgroundColor = {brightness, brightness, brightness, alpha};
		clear_pixels({brightness, brightness, brightness, alpha}); 
		};

	void background(u_char r, u_char g, u_char b) 
		{ m_backgroundColor = { r, g, b, 255 }; };
		
	void background(const Rgba & color) 
		{ m_backgroundColor = color;
		clear_pixels(color); };

	
	// getters
	Rgba background() const { return m_backgroundColor; };

	void draw();
	
	void attach(Shape & s) { this->context.attach(s); };
	void detach(Shape & s) { this->context.detach(s); };

	Context context; 

private:
	std::vector<Shape *> m_shapes;
	Rgba m_backgroundColor = {0, 0, 0, 255};
};
};

#endif
#ifndef SCREEN_HPP
#define SCREEN_HPP

#include <SDL2/SDL.h>
#include <string>
#include <iostream>
#include <vector>

#include "V2d.hpp"
#include "rgba.hpp"
#include "frame.hpp"

namespace fbg {
class SDL_Handler {
public:
	SDL_Handler();
	SDL_Handler(const std::string & title, const V2d<int> & start_point, int w, int h);
	SDL_Handler(const std::string & title, int w, int h);
	~SDL_Handler();

	int width()  const { return m_width; };
	int height() const { return m_height; };
	int size()   const { return m_width * m_height; };
	
	V2d<int> dimensions() const { return { m_width, m_height }; }
	

	const std::vector<int> & get_pressed_keys() const { return m_keys_down; };
	bool is_key_pressed(int key_id) const;
	bool is_key_pressed() const;
	V2d<int> mouse() const;
	V2d<int> mouse_pos_clamped() const;

	bool is_open() const { return m_is_open; };

protected:
	const Rgba & get_pixel(unsigned int n) 					  const { return frame.pixels[n]; };
	const Rgba & get_pixel(unsigned int x, unsigned int y)  const { return frame.pixels[y   * width() + x  ]; };
	const Rgba & get_pixel(const V2d<int> p)					  const { return frame.pixels[p.y * width() + p.x]; };

	void set_pixel(const V2d<int> & p, const Rgba & px);

	// list of scancodes corresponding to every key, represented by SDL
	std::vector<int> m_keys_down;
	bool m_is_open = false;

	void handle_event(const SDL_Event * e);
	void poll_events();
	void update();

	bool has_keyboard_focus() const;
	bool has_mouse_focus() const;

	void clear_pixels();
	void clear_pixels(const Rgba & col);
	
	Frame frame; // container for all pixels on screen

private:
	SDL_Window * m_window;
	SDL_Renderer * m_renderer;
	SDL_Texture * m_texture;

	const int m_width, m_height;

	void push_frame(const Frame & f);
};
};

#endif
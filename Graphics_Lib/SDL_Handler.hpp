#ifndef SCREEN_HPP
#define SCREEN_HPP

#include <SDL2/SDL.h>
#include <string>
#include <iostream>
#include <vector>

#include "V2d.hpp"
#include "rgba.hpp"

class SDL_Handler {
public:
	SDL_Handler();
	SDL_Handler(const std::string & title, const V2d<int> & start_point, unsigned int w, unsigned int h);
	SDL_Handler(const std::string & title, unsigned int w, unsigned int h);
	~SDL_Handler();

	int width()  const { return m_width; };
	int height() const { return m_height; };
	int size()   const { return m_width * m_height; };

	const std::vector<int> & get_pressed_keys() const { return m_keys_down; };
	bool is_key_pressed(int key_id) const;
	bool is_key_pressed() const;
	V2d<int> get_mouse_pos() const;
	V2d<int> get_mouse_pos_clamped() const;

	bool is_open() const { return m_is_open; };

protected:
	Rgba get_pixel(unsigned int n) 					  const { return m_pixels[n]; };
	Rgba get_pixel(unsigned int x, unsigned int y) const { return m_pixels[y   * width() + x  ]; };
	Rgba get_pixel(const V2d<int> p)					  const { return m_pixels[p.y * width() + p.x]; };

	void set_pixel(const V2d<int> & p, const Rgba & px);

	// list of scancodes corresponding to every key, represented by SDL
	std::vector<int> m_keys_down;
	bool m_is_open = false;

	void handle_event(const SDL_Event * e);
	void poll_events();
	void update();

	void clear_pixels();
	void clear_pixels(const Rgba & col);

private:
	SDL_Window * m_window;
	SDL_Renderer * m_renderer;
	SDL_Texture * m_texture;

	Rgba * m_pixels;
	const unsigned int m_width, m_height;

	void push_pixels(const Rgba * pxs);
};


#endif
#include "SDL_Handler.hpp"
#include <iostream>

SDL_Handler::SDL_Handler(const std::string & title, const V2d<int> & start_point, unsigned int w, unsigned int h) 
	: m_width { w }, m_height { h }
{
	// create SDL instances
	m_window = SDL_CreateWindow(title.c_str(), start_point.x, start_point.y, width(), height(), SDL_WINDOW_SHOWN);
	m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED);
	m_texture = SDL_CreateTexture(m_renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STATIC, width(), height());

	// check for error in initialization
	if (m_window == nullptr || m_renderer == nullptr || m_texture == nullptr)
		throw std::runtime_error("Error in initializing SDL window, renderer or texture" + (std::string) SDL_GetError());

	// allocate pixel buffer
	m_pixels = (Rgba *) malloc(sizeof(Rgba) * size());
	
	clear_pixels({0, 0, 0, 255});

	// set window to the open state
	m_is_open = true;
}

/*
	Creates window in the middle of the screen with the given width and height
*/
SDL_Handler::SDL_Handler(const std::string & title, unsigned int w, unsigned int h) 
	: SDL_Handler { title, {SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED}, w, h }
{ }

/*
	Default constructor -> applies sensible defaults.
*/
SDL_Handler::SDL_Handler() 
	: SDL_Handler { "Window", {SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED}, 640, 480 }
{ }

/*
	Destructor
*/
SDL_Handler::~SDL_Handler() {
	SDL_DestroyTexture(m_texture);
	SDL_DestroyRenderer(m_renderer);
	SDL_DestroyWindow(m_window);
}

/*
	Set given point to given pixel color value
*/
void SDL_Handler::set_pixel(const V2d<int> & p, const Rgba & px) {
	static int i = 0;
	m_pixels[p.y * width() + p.x] = px;
}

void SDL_Handler::clear_pixels(const Rgba & color) {
	std::fill(m_pixels, m_pixels + size(), color);
}

void SDL_Handler::clear_pixels() {
	memset(m_pixels, 0, sizeof(Rgba) * size());
}

void SDL_Handler::push_pixels(const Rgba * pxs) {
	if (SDL_UpdateTexture(m_texture, NULL, pxs, sizeof(Rgba) * width()) != 0)
		throw std::runtime_error("Error on updating texture.\n");
}

/*
	TODO: Add support for multiple windows
*/
void SDL_Handler::handle_event(const SDL_Event * e) {
	if (e->type == SDL_KEYDOWN && !is_key_pressed(e->key.keysym.sym)) {
		int scancode = SDL_GetScancodeFromKey(e->key.keysym.sym);
		m_keys_down.push_back(scancode); 
	}
	
	if (e->type == SDL_KEYUP)  {
		int scancode = SDL_GetScancodeFromKey(e->key.keysym.sym);
		for (int i = 0; i < m_keys_down.size(); i++)
			if (m_keys_down[i] == scancode)
				m_keys_down.erase(m_keys_down.begin() + i);
	}

	if (e->type == SDL_QUIT)
		m_is_open = false;
}

void SDL_Handler::poll_events() {
	SDL_Event e;
	while (SDL_PollEvent(&e))
		handle_event(&e);
}

void SDL_Handler::update() {

	if (!is_open()) 
		throw std::runtime_error("SDL_Handler error: Cannot update unopen screen.\n");
	
	poll_events();

	// push pixelbuffer to the screen
	push_pixels(m_pixels);
	// clear renderer
	SDL_RenderClear(m_renderer);
	// copy the texture into the renderer
	SDL_RenderCopy(m_renderer, m_texture, NULL, NULL);
	// display contents of renderer
	SDL_RenderPresent(m_renderer); 
}

bool SDL_Handler::is_key_pressed(int key_id) const {
	for (int key : m_keys_down)
		if (key == key_id)
			return true;
	return false;
}

bool SDL_Handler::is_key_pressed() const {
	return !(m_keys_down.size() == 0);
}

/*
	Get the mouse position relative to the top left corner of the screen.
*/
V2d<int> SDL_Handler::get_mouse_pos() const {
	SDL_PumpEvents();
	V2d<int> res;
	V2d<int> window_pos;
	SDL_GetGlobalMouseState(&res.x, &res.y);
	SDL_GetWindowPosition(m_window, &window_pos.x, &window_pos.y);
	res = res - window_pos;
	return res;
}

/*
	Clamp point p inside bounding box defined by points f and t (inclusive).
*/
void clamp_point(V2d<int> & p, const V2d<int> & f, const V2d<int> & t) {
	// clamp horisontally
	if (p.x < f.x) p.x = f.x;
	else if (p.x > t.x) p.x = t.x;

	// clamp vertically
	if (p.y < f.y) p.y = f.y;
	else if (p.y > t.y) p.y = t.y;
}

/*
	TODO: Add support for multiple windows
*/
V2d<int> SDL_Handler::get_mouse_pos_clamped() const {
	V2d<int> res = get_mouse_pos();
	// clamp mouse position within screen space
	clamp_point(res, {0, 0}, {width() - 1, height() - 1});
	return res;
};

/*
	Returns whether this window is in keyboard focus.
	TODO: Untested.
	Update: doesnt work
*/
bool SDL_Handler::has_keyboard_focus() const {
	return SDL_GetKeyboardFocus() == m_window;
}

/*
	Returns whether this window is in mouse focus.
	TODO: Untested
*/
bool SDL_Handler::has_mouse_focus() const {
	return SDL_GetMouseFocus() == m_window; 
}
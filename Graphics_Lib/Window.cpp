#include "Window.hpp"
#include "alphacomposite.hpp"

#define KEY_DELAY 10

using namespace fbg;

void Window::wait_for_key() 
{
   while (m_keysDown.size() == 0 || !this->has_keyboard_focus()) {
      SDL_Event e;
      SDL_WaitEvent(&e);
      handle_event(&e);
   }
}

void Window::wait_for_key(int key_id) 
{
   while (!key_is_pressed(key_id) || !this->has_keyboard_focus()) {
      SDL_Event e;
      SDL_WaitEvent(&e);
      handle_event(&e);
   }
}

void Window::update() 
{
   // Alpha composite entire pixel area using AVX2 vector processing 
   alpha_compositeNC(frame.pixels, &m_backgroundColor, size());
   
   // change m_pixels by adding the colors of the strokes and fills within
   // the shapes of the context object
   context.draw_fill(frame);
   
   // update the SDL Pixelbuffer and pull new events.
   SDLHandler::update_pixels();
}
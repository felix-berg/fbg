#ifndef SCREEN_HPP
#define SCREEN_HPP

#include <SDL.h>
#include <string>
#include <iostream>
#include <vector>

#include "V2d.hpp"
#include "rgba.hpp"
#include "frame.hpp"

namespace fbg {
   class SDLHandler {
   public:
      SDLHandler(const std::string & title, const V2d<int> & startPoint, int w, int h);
      SDLHandler(const std::string & title, int w, int h);

      ~SDLHandler();

      SDLHandler(const SDLHandler &) = delete;
      SDLHandler & operator = (const SDLHandler &) = delete;
      SDLHandler(SDLHandler &&) = delete;
      SDLHandler & operator = (SDLHandler &&) = delete;

      int width()  const { return m_width; };
      int height() const { return m_height; };
      int size()   const { return m_width * m_height; };

      V2d<int> dimensions() const { return { m_width, m_height }; }

      const std::vector<int> & pressedKeys() const { return m_keysDown; };
      bool key_is_pressed(int key_id) const;
      bool key_is_pressed() const;
      V2d<int> mouse() const;
      V2d<int> mouse_pos_clamped() const;

      bool isOpen() const { return m_isOpen; };

   protected:
      Rgba get_pixel(u_int n)          const { return frame.pixels[n]; };
      Rgba get_pixel(u_int x, u_int y) const { return frame.get(x, y); };

      void set_pixel(const V2d<int> & p, const Rgba & px);

      // list of scancodes corresponding to every key, represented by SDL
      std::vector<int> m_keysDown;
      bool m_isOpen = false;

      void handle_event(const SDL_Event * e);
      void poll_events();
      void update_pixels();

      bool has_keyboard_focus() const;
      bool has_mouse_focus() const;

      void clear_pixels();
      void clear_pixels(Rgba);
      
      Frame frame; // container for all pixels on screen

   private:
      SDL_Window * m_window;
      SDL_Renderer * m_renderer;
      SDL_Texture * m_texture;

      const int m_width, m_height;

      void push_frame(const Frame  & f);
   };
};

#endif
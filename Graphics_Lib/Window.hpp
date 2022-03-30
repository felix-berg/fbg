#ifndef WINDOW_HPP
#define WINDOW_HPP

#include "Shapes/shape.hpp"
#include "Shapes/context.hpp"
#include "SDL_Handler.hpp"

#include <vector>

namespace fbg {
   /** Window class. Used for rendering shapes. 
    * @param attach(): Attach a shape to be drawn to the screen. 
    * @param detach(): Detach a shape from the screen.
    * @param draw(): Draw the attached shapes to the window.
    * @param background(): Set background color of window. 
    * @param wait_for_key(): Wait for key-press before continuing. */
   class Window : public SDLHandler {
   public:
      /** Constructor for window class 
       * @param title: The title to be displayed to the top bar of the screen. 
       * @param w: The width of the window.
       * @param h: The height of the window. */
      Window(const std::string & title, int w, int h) : SDLHandler { title, w, h } {	};

      /** Default constructor for window.
       * Sets the title to "Window". 
       * Sets the width to 640 and the height to 480. */
      Window() : SDLHandler { "Window", {SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED}, 640, 480 } {	};
      
      /** Setter for background color.
       * @param b: Grayscale value for background color. [0, 255] */
      void background(u_char b) { 
         m_backgroundColor = {b, b, b, 255}; 
      };

      /** Setter for background color.
       * @param b: Grayscale value for background color. [0, 255]
       * @param a: Alpha value for background. [0, 255] */
      void background(u_char brightness, u_char alpha) { 
         m_backgroundColor = {brightness, brightness, brightness, alpha};
         clear_pixels({brightness, brightness, brightness, alpha}); 
      };

      /** Setter for background color.
       * @param r: Red-value of background color. [0, 255]
       * @param g: Green-value of background color. [0, 255]
       * @param b: Blue-value of background color. [0, 255] */
      void background(u_char r, u_char g, u_char b) { 
         m_backgroundColor = { r, g, b, 255 }; 
      };

      /** Setter for background color.
       * @param r: Red-value of background color. [0, 255]
       * @param g: Green-value of background color. [0, 255]
       * @param b: Blue-value of background color. [0, 255] 
       * @param a: Alpha-value of background color. [0, 255]*/
      void background(u_char r, u_char g, u_char b, u_char a) { 
         m_backgroundColor = { r, g, b, a }; 
      };		
         
      /** Setter for background color.
       * @param color: The color to set to the background. */
      void background(Rgba color) { 
         m_backgroundColor = color;
         clear_pixels(color); 
      };

      /** Getter for background color.
       * @return Color of background. */
      Rgba background() const { return m_backgroundColor; };

      /** Render the currently attached shapes to the window */
      void update();
      
      /** Attach shape to window. 
       * @param s: The shape to be attached to the window. */
      void attach(Shape & s) { 
         this->context.attach(s); 
      };

      /** Detach shape from window.
       * @param s: The shape to be detached from the window. */
      void detach(Shape & s) { this->context.detach(s); };

      /** Stop execution until a given key is pressed. 
       * @param key_id: The key to be pressed for execution to continue. */
      void wait_for_key(int key_id); // TODO: Make own key enum

      /** Stop execution until any key is pressed. */
      void wait_for_key();


   private:
      Rgba m_backgroundColor {140, 140, 140, 255};
      
      /** The context of the window. 
       * Holds every shape, that is attached to the window.  */
      Context context; 
   };
};

#endif
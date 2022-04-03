#ifndef RGBA_HPP
#define RGBA_HPP

#include <iostream>
#include <cstdint>
#include <SDL2/SDL.h>
using std::ostream;

/*
   A color given by a red, green, blue and alpha value.
*/
namespace fbg {
   /** Color class. 
    * @param r: Red value of color. [0, 255] 
    * @param g: Green value of color. [0, 255] 
    * @param b: Blue value of color. [0, 255] 
    * @param a: Alpha value of color. [0, 255] */ 
   struct Rgba {
      /** 8-bit color value. [0, 255] */
      uint8_t a, b, g, r; 
      // must be opposite because of conversion to u_int.

      Rgba() { };
      Rgba(uint8_t rr, uint8_t gg, uint8_t bb, uint8_t aa)
         : r { rr }, g { gg }, b { bb }, a { aa } { };
   }; 

   ostream & operator << (ostream & os, Rgba color);
};
#endif
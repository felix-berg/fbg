#ifndef PIXELGRID_HPP
#define PIXELGRID_HPP

#include <cstring> // memset
#include <iostream>
#include <string>

#include "rgba.hpp"
#include "alphacomposite.hpp"

/*
   Container for Rgba pixelbuffer that includes width and height
*/
namespace fbg {
   struct Frame { 
      struct OutOfRange : std::runtime_error {
         OutOfRange(int x, int y) 
            : std::runtime_error { "Position (" + std::to_string(x) + ", " + std::to_string(y) + ") is out of range." } { };
      };

      Rgba * pixels;
      const int w, h;

      Frame(int w_, int h_) 
         : w { w_ }, h { h_ }
      {
         pixels = new Rgba[w_ * h_];
         memset(pixels, 0, sizeof(Rgba) * w * h);
      }

      ~Frame() { delete pixels; };

      Frame(const Frame & other) 
         : w { other.w }, h { other.h }, pixels { new Rgba[other.w * other.h] }
      {
         memcpy(pixels, other.pixels, w * h * sizeof(Rgba));
      }

      Frame(Frame && other)
         : w { other.w }, h { other.h }, pixels { other.pixels }
      {
         other.pixels = nullptr;
      }

      /*	Draw color to pixel at the given x, y coordinate.
         Doesn't draw, if the point is out of bounds. */
      void set_pixel(int x, int y, Rgba color) 
      {
         if (in_bound(x, y)) 
            alpha_composite1(&pixels[y * w + x], &color);
      }

      /*	
         Returns a boolean indicating whether the point (x, y) is
         within the bounds of (lx, ly) to (ux, uy).
         Inclusive!
      */
      bool in_bound(int x, int y) 
      {
         return ! (x < 0 || x >= w ||
                    y < 0 || y >= h);
      }

      Rgba get(int x, int y) const { 
         if (x < 0 || x >= w || y < 0 || y >= h)
            throw OutOfRange(x, y);
         
         return pixels[y * w + x]; 
      };

      void add(const Frame & other, int tx, int ty);
   };
};

#endif
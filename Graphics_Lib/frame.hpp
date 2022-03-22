#ifndef PIXELGRID_HPP
#define PIXELGRID_HPP

#include "rgba.hpp"
#include "alphacomposite.hpp"

/*
   Container for Rgba pixelbuffer that includes width and height
*/
namespace fbg {
   struct Frame { 
      Rgba * pixels;

      const int w, h;

      Frame(int w, int h) 
         : w { w }, h { h }
      {
         pixels = (Rgba *) malloc(sizeof(Rgba) * w * h);
         memset(pixels, 0, sizeof(Rgba) * w * h);
      }

      ~Frame() {
         if (pixels != nullptr) 
            free(pixels);
      }

      /*	Draw color to pixel at the given x, y coordinate.
         Doesn't draw, if the point is out of bounds. */
      void set_pixel(int x, int y, const Rgba & color) {
         if (in_bound(x, y)) 
            alpha_composite1(&pixels[y * w + x], &color);
      }

      /*	
         Returns a boolean indicating whether the point (x, y) is
         within the bounds of (lx, ly) to (ux, uy).
         Inclusive!
      */
      bool in_bound(int x, int y) {
         return ! (x < 0 || x >= w ||
                    y < 0 || y >= h);
      }
   };
};

#endif
#include "frame.hpp"

#include "Shapes/drawing_algorithms/drawpixel.hpp"


using namespace fbg;

void Frame::add(const Frame & other, int tx, int ty)
{
   if (ty + other.h < 0 || ty >= this->h || tx + other.w < 0 || tx >= this->w)
      return;

   // restrict line bounds within frame
   int lineLength = other.w;

   int ox = 0;

   if (tx < 0) {
      lineLength += tx;
      ox -= tx;
      tx = 0;
   }

   if (tx + lineLength >= this->w) {
      lineLength -= (tx + lineLength) - this->w;
   }
   
   // scan thru y of other
   for (int oy = 0; oy < other.h; oy++) {
      int thisy = oy + ty;
      if (thisy < 0 || thisy >= this->h) continue; // bounds check

      // draw from the start pixel + w * y to the others pixel
      alpha_compositeN(&this->pixels[tx + thisy * this->w],
                       &other.pixels[ox + oy * other.w], 
                       lineLength);
   }
}
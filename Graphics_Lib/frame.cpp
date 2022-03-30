#include "frame.hpp"

#include "Shapes/drawing_algorithms/drawpixel.hpp"


using namespace fbg;

void Frame::add(const Frame & other, int tx, int ty)
{
   // restrict line bounds within frame
   int lineLength = other.w;
   if (tx < 0) {
      lineLength += tx;
      tx = 0;
   }

   if (tx + lineLength >= other.w)
      lineLength -= (tx + lineLength) - other.w;

   // index for starting pixels for "this" and the "other" frame         
   int thisFromIdx =  tx + this->w * ty;
   int otherFromIdx = 0;

   // scan thru y of other
   for (int oy = 0; oy < other.h; oy++) {
      std::cout << "scan " << other.pixels[oy * other.w] << '\n';
      int thisy = ty + oy; // convert to y in relation to this

      if (thisy < 0 || thisy >= this->h) continue; // bounds check

      // draw from the start pixel + w * y to the others pixel
      alpha_compositeN(this->pixels + thisFromIdx  + thisy * this->w,
                       other.pixels + otherFromIdx + oy    * other.w, 
                       lineLength);
   }
}
#include "drawpixel.hpp"
#include "drawcircle.hpp"
#include "../../alphacomposite.hpp"

using namespace fbg;

/** Compute pixels of given horisontal line, making sure, that no buffer overflow occurs.
 * Inclusive of tx */
void fbg::set_horisontal_line(Frame & frame, int fx, int tx, int y, Rgba color) 
{
   int lLen = tx - fx + 1; // length of line

   // y value
   if (fx + lLen < 0 || fx >= frame.w || y < 0 || y >= frame.h) {
      lLen = 0;
      } else {
         // x value
         if (fx < 0) {
         // lessen number of pixels in width (dont change the length of the line, just how many pixels are drawn of it)
         lLen += fx;

         // move the startindex to the first column of pixels
         fx = 0;
      }

      if (fx + lLen >= frame.w) 
         lLen -= (fx + lLen) - frame.w;

      int pIdx = frame.w * y + fx; // pixel index for top left pixel of rectangle

      // composite pixels on top with the given pixel_id and line lengths
      alpha_compositeNC(&frame.pixels[pIdx], &color, lLen);
   }
}

void fbg::set_vertical_line(Frame & frame, int x, int fy, int ty, Rgba color) 
{
   for (int y = fy; y <= ty; y++)
      frame.set_pixel(x, y, color);
}

/*
   TODO: Untested
*/
void fbg::draw_pixel(Frame & frame, int x, int y, Rgba color, int sw) 
{
   if (sw == 1)
      frame.set_pixel(x, y, color);
   else
      compute_circle_fill(frame, x, y, sw, color);
}

/*
   Get the right and left pixel offsets based on given
   strokeweight
*/
void fbg::offsets_from_strokeweight(int sw, int * lOff, int * rOff) 
{
   if (sw % 2 == 0) { // even
      *lOff = sw / 2;
      *rOff = sw / 2 - 1;
   } else { // odd
      *lOff = (sw - 1) / 2;
      *rOff = *lOff;
   }
}

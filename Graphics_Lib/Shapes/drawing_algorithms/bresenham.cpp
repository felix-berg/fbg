#include "bresenham.hpp"

/*	Bresenhams line algorithm: https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm
   Case for when absolute change in x is greater than absolute change in y. abs(gradient) < 1.
   Stepping along x-axis -> only one point per column. 
   Calls set(x, y) for every point it finds along the way. 
   set(x, y) needs to return true, if the algorithm is to continue.*/ 
void fbg::bresenham_line(int fx, int fy, int tx, int ty, std::function<bool(int x, int y)> set) 
{
   int dx = tx - fx;
   int dy = ty - fy;

   int yDir = 1;
   if (dy < 0) {
      yDir = -1;
      dy *= -1;
   }

   // calculate the first error absolutely -> rest are calculated accumulatively
   int D  = 2 * dy - dx;
   int y  = fy;

   // loop along x-axis: from.x -> to.y
   for (int x = fx; x <= tx; x++) {
      // set pixel and break if wanted
      if (!set(x, y)) break;

      if (D > 0) { // the point is under the line, the error is positive. Step one pixel vertically
         y += yDir;
         D -= 2 * dx;
      }
      // no vertical step
      D += 2 * dy;
   }
}

/** This function draws single pixels in the first quadrant, and expects that set() will handle the other seven. */ 
void fbg::bresenham_circle(int r, std::function<void(int x, int y)> set) 
{
   int x = r;
   int y = 0;

   int D = 3 - 2 * r;

   while (x >= y) {
      set(x, y);

      if (D > 0) {
         D = D + 4 * (y - x) + 10;
         x = x - 1;
      }
      else // D <= 0:
         D = D + 4 * y + 6;

      y = y + 1;
   }
}

#include "Shapes/drawing_algorithms/drawpolyline.hpp"
#include "Shapes/drawing_algorithms/bresenham.hpp"
#include "Shapes/drawing_algorithms/drawline.hpp"
#include "Shapes/drawing_algorithms/drawpixel.hpp"

#include <functional>

using namespace fbg;

void fill_stroke_patterns_minmax(std::vector<int> & minPattern, std::vector<int> & maxPattern, int fx, int fy, int tx, int ty, int arrayBeginIdx) 
{
   if (std::abs(tx - fx) > std::abs(ty - fy)) {
      auto setpx = [&](int x, int y) -> bool 
      { 
         int patternY = y - arrayBeginIdx;

         if (x < minPattern[patternY]) minPattern[patternY] = x;
         if (x > maxPattern[patternY]) maxPattern[patternY] = x;

         return true;
      };

      if (fx <= tx)
         bresenham_line(fx, fy, tx, ty, setpx);
      else 
         bresenham_line(tx, ty, fx, fy, setpx);

   } else {
      auto setpx = [&](int x, int y) -> bool // in reverse
      {
         int patternY = x - arrayBeginIdx;

         if (y < minPattern[patternY]) minPattern[patternY] = y;
         if (y > maxPattern[patternY]) maxPattern[patternY] = y;

         return true;
      };

      if (fy <= ty)
         bresenham_line(fy, fx, ty, tx, setpx);
      else
         bresenham_line(ty, tx, fy, fx, setpx);
   }
}

void fbg::compute_polyline_convex_fill(Frame & frame, std::vector<V2d<float>> lst, Rgba color) 
{
   if (lst.size() <= 0) return;

   static std::vector<int> minPattern;
   static std::vector<int> maxPattern;

   int minY = INT32_MAX, 
       maxY = INT32_MIN;

   for (const V2d<int> & p : lst) {
      if (p.y < minY) minY = p.y;
      if (p.y > maxY) maxY = p.y;
   }

   int patternHeight = maxY - minY + 1;

   minPattern.resize(patternHeight);
   std::fill(minPattern.begin(), minPattern.end(), INT32_MAX);

   maxPattern.resize(patternHeight);
   std::fill(maxPattern.begin(), maxPattern.end(), INT32_MIN);

   V2d<int> to, from = lst.back();

   for (int i = 0; i < lst.size(); i++) {
      to = lst[i];
      fill_stroke_patterns_minmax(minPattern, maxPattern, from.x, from.y, to.x, to.y, minY);
      from = to;
   }

   for (int y = 1; y < patternHeight - 1; y++)
      set_horisontal_line(frame, minPattern[y] + 1, maxPattern[y] - 1, y + minY, color);
}

#include "drawpolyline.hpp"
#include "bresenham.hpp"
#include "drawline.hpp"
#include "drawpixel.hpp"

#include <functional>

using namespace fbg;

void fill_stroke_patterns_minmax(std::vector<int> & minPattern, std::vector<int> & maxPattern, int fx, int fy, int tx, int ty, int arrayBeginIdx) {
   bool low = true;

   int fromX = fx, fromY = fy;
   int toX   = tx, toY   = ty;

   std::function<int(int, int)> getx;
   std::function<int(int, int)> gety;
   
   if (fromX > toX) {
      fromX = tx;
      fromY = ty;
      toX   = fx;
      toY   = fy;
   }

   if (toX - fromX > toY - fromY) {
      toX = tmpX; toY = tmpY;
      getx = [](int x, int y) -> int { return x; };
      gety = [](int x, int y) -> int { return y; };
   } else {
      getx = [](int x, int y) -> int { return y; };
      gety = [](int x, int y) -> int { return x; };
      int tmp1 = fromX;
      int tmp2 = toX;
      fromX = fromY; fromY = tmp1;
      toX = toY;     toY = tmp2;
   }

   bresenham_line(fromX, fromY, toX, toY, [&](int x, int y) -> bool {
      int patternY = gety(x, y) - arrayBeginIdx;

      if (getx(x, y) < minPattern[patternY]) minPattern[patternY] = getx(x, y);
      if (getx(x, y) > maxPattern[patternY]) maxPattern[patternY] = getx(x, y);

      return true;
   });
}

void fbg::compute_polyline_convex_fill(Frame & frame, std::vector<V2d<float>> lst, const Rgba & color) 
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

   for (int y = 0; y < patternHeight; y++)
      set_horisontal_line(frame, minPattern[y], maxPattern[y], y + minY, color);
}
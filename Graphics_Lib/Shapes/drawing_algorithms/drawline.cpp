#include "drawline.hpp"
#include "drawpixel.hpp"
#include "drawcircle.hpp"
#include "bresenham.hpp"

#include <vector>

using namespace fbg;

// TODO: Lines with even strokeweight look a little goofy

void draw_stroke_low(Frame & frame, int fx, int fy, int tx, int ty, const Rgba & color, int sw, bool smoothEdges); 
void draw_stroke_hi(Frame & frame, int fx, int fy, int tx, int ty, const Rgba & color, int sw, bool smoothEdges);

/*
   Compute the colors of the lines stroke and output onto "frame".
   Line defined by fx, fy -> tx, ty (inclusive).
   This function doesn't make smooth edges.
*/
void fbg::compute_line_stroke(Frame & frame, int fx, int fy, int tx, int ty, const Rgba & color, int sw) {
   if (abs(tx - fx) > abs(ty - fy)) { // if gradient < 1
      if (fx > tx) // if points are the opposite way round
         draw_stroke_low(frame, tx, ty, fx, fy, color, sw, false);
      else 
         draw_stroke_low(frame, fx, fy, tx, ty, color, sw, false);
   } else // if gradient > 1, use "hi"
      if (fy > ty)
         draw_stroke_hi(frame, tx, ty, fx, fy, color, sw, false);
      else
         draw_stroke_hi(frame, fx, fy, tx, ty, color, sw, false);
}

void fbg::compute_line_stroke_smooth(Frame & frame, int fx, int fy, int tx, int ty, const Rgba & color, int sw) {
   if (abs(tx - fx) > abs(ty - fy)) { // if gradient < 1
      if (fx > tx) // if points are the opposite way round
         draw_stroke_low(frame, tx, ty, fx, fy, color, sw, sw != 1);
      else 
         draw_stroke_low(frame, fx, fy, tx, ty, color, sw, sw != 1);
   } else // if gradient > 1, use "hi"
      if (fy > ty)
         draw_stroke_hi(frame, tx, ty, fx, fy,  color, sw, sw != 1);
      else
         draw_stroke_hi(frame, fx, fy, tx, ty,  color, sw, sw != 1);
}


/** Get coordinates for point of perpendicular line within grid.
 * Translates from (x, y) into (&px, &py).  */
void get_perpendicular_line_point(int x, int y, int * px, int * py, int gridWidth, int gradientSign) {
   bool isNegative = gradientSign < 0;

   int lx = isNegative * (gridWidth - 1) + gradientSign * x;
   int ly = y * gradientSign;

   *px = ly;
   *py = gridWidth - 1 - lx;
}

/** Make sure, that min and max are in the right order */
inline void fix_min_max(int * min, int * max) {
   if (*min > *max) { 
      int tmp = *min;
      *min = *max;
      *max = tmp;
   }
}


/** Get list filled with the x-values of the perpendicular line to the line defined by (fx, tx). 
 * @returns Reference to static s_strokePattern vector. */
std::vector<int> & get_stroke_pattern(int fx, int fy, int tx, int ty, int patternHeight) {
   static std::vector<int> s_strokePattern; // container for current stroke pattern
   
   s_strokePattern.resize(patternHeight); // set the size of the container to relevant size

   std::fill(s_strokePattern.begin(), s_strokePattern.end(), -1);

   int gradientSign = 2 * (ty - fy >= 0) - 1;

   int cx = patternHeight / 2;
   int cy = cx;
   int midx;

   bresenham_line(0, 0, tx - fx, ty - fy, [&](int x, int y) -> bool {
      if (x >= patternHeight) return false; // if we have evaluated enough, stop the algorithm
      int px, py;
      get_perpendicular_line_point(x, y, &px, &py, patternHeight, gradientSign);

      s_strokePattern[py] = px;

      if (py == cy)
         midx = px;

      return true; // continue algorithm
   });


   // move every bit of the perpendicular line grid, 
   // so that the middle pixel of the line is in the center
   int perpLineOffset = cx - midx; // the amount to offset every pixel
   for (int y = 0; y < patternHeight; y++)
      s_strokePattern[y] += perpLineOffset;

   // sort min and max values
   int minX = s_strokePattern[0];
   int maxX = s_strokePattern[patternHeight - 1];

   fix_min_max(&minX, &maxX);

   // calculate gradient of line
   int lineX = maxX - minX;
   float gradient = static_cast<float> (patternHeight) / static_cast<float> (lineX);

   // compute appropriate grid height based on the line reach X
   int lineY = static_cast<int> (std::sin(std::atan(gradient)) * (patternHeight)); // sin(atan(gradient)) * strokeweight

   // remove top and bottom lines according to gradient
   int n_linesToRemove = patternHeight - lineY;
   for (int y = 0; y < n_linesToRemove / 2; y++) {
      s_strokePattern[y] 							= -1;
      s_strokePattern[patternHeight - 1 - y] = -1;
   }

   return s_strokePattern;
}

/** Find gaps in given stroke pattern and return a stroke pattern of the necessary points to fill the gaps,
 * when the y-direction changes in the bresenham line algorithm.
  * @returns a reference to the static stroke pattern for gaps. */
std::vector<int> & get_gaps_from_stroke_pattern(const std::vector<int> & strokePattern, int patternHeight, int gradientSign){
   static std::vector<int> s_gapStrokePattern;

   s_gapStrokePattern.resize(patternHeight);
   std::fill(s_gapStrokePattern.begin(), s_gapStrokePattern.end(), -1);

   // compare strokePattern to itself, but shifted as if the y-direction changed
   for (int y = 0; y < patternHeight; y++) {
      int x = strokePattern[y];

      // if the stroke pattern is undefined for the given y-line
      if (x == -1) {
         s_gapStrokePattern[y] = -1;
         continue; 
      }

      // the shifted x and y-values
      int shiftedY = y + gradientSign;

      if (shiftedY < 0 || shiftedY >= patternHeight) { // shiftedY is off-bounds
         s_gapStrokePattern[y] = -1;
         continue; 
      }

      // x = x - 1
      int shiftedX = strokePattern[shiftedY] - 1;

      // check for |X|gap|X|
      if (shiftedX + 2 == x)
         s_gapStrokePattern[y] = shiftedX + 1;
      else
         s_gapStrokePattern[y] = -1;		
   }

   return s_gapStrokePattern;
}


/** Create a stroke pattern of a circle for ends of lines with the given strokeweight
 *  @returns Reference to static s_circleStrokePattern vector. */
std::vector<std::pair<int, int>> & get_circle_stroke_pattern(int sw) {
   static std::vector<std::pair<int, int>> s_circleStrokePattern;

   s_circleStrokePattern.resize(sw);
   std::fill(s_circleStrokePattern.begin(), s_circleStrokePattern.end(), std::make_pair(-1, -1));

   int r = sw / 2;
   bresenham_circle(r, [&](int circx, int circy) -> void {
      int y1 = r + circy, y2 = r - circy,
          y3 = r + circx, y4 = r - circx;

      s_circleStrokePattern[y1].first  = r - circx;
      s_circleStrokePattern[y1].second = r + circx;
      s_circleStrokePattern[y2].first  = r - circx;
      s_circleStrokePattern[y2].second = r + circx;
      s_circleStrokePattern[y3].first  = r - circy;
      s_circleStrokePattern[y3].second = r + circy;
      s_circleStrokePattern[y4].first  = r - circy;
      s_circleStrokePattern[y4].second = r + circy;
   });
   return s_circleStrokePattern;
}

void draw_stroke_pattern(Frame & frame, const std::vector<int> & strokePattern, int cx, int cy, std::function<void(int x, int y)> setpx) {
   int patternSz = strokePattern.size();
   for (int y = 0; y < patternSz; y++) {
      if (strokePattern[y] == -1) continue;
      setpx(cx + strokePattern[y] - patternSz / 2, cy + y - patternSz / 2);
   }
}

void draw_thick_line(Frame & frame, int fx, int fy, int tx, int ty, const std::vector<int> & strokePattern, const std::vector<int> & gapPoints, std::function<void(int x, int y)> setpx) {
   int prevy = fy;
   bresenham_line(fx, fy, tx, ty, [&](int x, int y) -> bool {
      if (y != prevy) // if we stepped down fill the gaps
         draw_stroke_pattern(frame, gapPoints, x, y, setpx);

      draw_stroke_pattern(frame, strokePattern, x, y, setpx);

      prevy = y;
      return true;
   });
}

void draw_stroke_line_low_templated(Frame & frame, int fx, int fy, int tx, int ty, int sw, bool smoothEdges, std::function<void(int x, int y)> setpx, std::function<void(int fx, int tx, int y)> sethoriline) {
   int patternHeight = sw;
   int gradientSign = (ty - fy >= 0) * 2 - 1;

   // this is a pointer to a global resource
   std::vector<int> & strokePattern = get_stroke_pattern(fx, fy, tx, ty, patternHeight);
   // get the gap-points for the given stroke pattern
   std::vector<int> & gapPoints = get_gaps_from_stroke_pattern(strokePattern, patternHeight, gradientSign);
   // circle for given stroke weight
   std::vector<std::pair<int, int>> & circleStrokePattern = get_circle_stroke_pattern(patternHeight);

   draw_thick_line(frame, fx, fy, tx, ty, strokePattern, gapPoints, setpx);

   if (!smoothEdges) return;

   // ------ DRAWING SMOOTH EDGES -----
   int centerX = sw / 2;
   int centerY = sw / 2;

   for (int y = 0; y < patternHeight; y++) {
      // sort out empty lines. Special case
      if (strokePattern[y] == -1 && (gradientSign == 1) == (y < centerY)) continue;

      // get the x-value of the pixel farthest to the left, and farthest to
      // the right in the y'th row.
      int endLeftX;
      if (strokePattern[y] == -1) {
         endLeftX = tx + circleStrokePattern[y].first - centerX;
      } else {
         endLeftX = tx + std::max(strokePattern[y], circleStrokePattern[y].first) - centerX;
      }

      // the farthest away pixel will always be the second circle pixel in the row
      int endRightX = tx + circleStrokePattern[y].second - centerX;
      int endY = ty + y - centerY;

      // set the computed line of the frame. +1 since we need to avoid overlap.
      sethoriline(endLeftX + 1, endRightX, endY);
   }

   for (int y = 0; y < patternHeight; y++) {
      // sort out empty lines. Special case
      if (strokePattern[y] == -1 && (gradientSign == 1) == (y >= centerY)) continue;

      // get the x-value for the pixel furthest to the right in the stroke pattern and circle pattern
      int startRightX;
      if (strokePattern[y] == -1) 
         startRightX = fx + circleStrokePattern[y].second - centerX;
      else
         startRightX = fx + std::min(strokePattern[y], circleStrokePattern[y].second) - centerX;

      // the leftmost pixel is always the circle
      int startLeftX = fx + circleStrokePattern[y].first - centerX;

      int startY = fy + y - centerY;

      sethoriline(startLeftX, startRightX - 1, startY); // -1 avoids overlap
   }
}

void draw_stroke_low(Frame & frame, int fx, int fy, int tx, int ty, const Rgba & color, int sw, bool smoothEdges) {
   std::function<void(int, int)> setpx = [&](int x, int y) -> void {
      frame.set_pixel(x, y, color);   
   };
   
   std::function<void(int, int, int)> sethoriline = [&](int fx, int tx, int y) -> void {
      set_horisontal_line(frame, fx, tx, y, color);
   };

   draw_stroke_line_low_templated(frame, fx, fy, tx, ty, sw, smoothEdges, setpx, sethoriline);
}

void draw_stroke_hi(Frame & frame, int fx, int fy, int tx, int ty, const Rgba & color, int sw, bool smoothEdges) {
   // use the draw_stroke_line_low_templated, but switch x with y in the drawing functions.
   std::function<void(int, int)> setpx = [&](int x, int y) -> void {
      frame.set_pixel(y, x, color);   
   };
   
   std::function<void(int, int, int)> sethoriline = [&](int fx, int tx, int y) -> void {
      for (int x = fx; x <= tx; x++)
         frame.set_pixel(y, x, color);
   };

   draw_stroke_line_low_templated(frame, fy, fx, ty, tx, sw, smoothEdges, setpx, sethoriline);
}
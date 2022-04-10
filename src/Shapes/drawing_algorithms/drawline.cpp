#include "Shapes/drawing_algorithms/drawline.hpp"
#include "Shapes/drawing_algorithms/drawpixel.hpp"
#include "Shapes/drawing_algorithms/drawcircle.hpp"
#include "Shapes/drawing_algorithms/bresenham.hpp"

#include <vector>

using namespace fbg;

constexpr int NONE = INT32_MIN;

void draw_stroke_low(Frame & frame, int fx, int fy, int tx, int ty, Rgba color, int sw, LineMode mode); 
void draw_stroke_hi (Frame & frame, int fx, int fy, int tx, int ty, Rgba color, int sw, LineMode mode);

/** Compute the colors of the lines stroke and output onto "frame".
 * Line defined by fx, fy -> tx, ty (inclusive). 
 * This function doesn't make smooth edges.     */
void fbg::compute_line_stroke(Frame & frame, int fx, int fy, int tx, int ty, Rgba color, int sw, LineMode mode) 
{
   if (abs(tx - fx) > abs(ty - fy)) { // if gradient < 1
      if (fx > tx) // if points are the opposite way round
         draw_stroke_low(frame, tx, ty, fx, fy, color, sw, mode); // inverted
      else 
         draw_stroke_low(frame, fx, fy, tx, ty, color, sw, mode);
   } else // if gradient > 1, use "hi"
      if (fy > ty)
         draw_stroke_hi (frame, tx, ty, fx, fy, color, sw, mode); // inverted
      else
         draw_stroke_hi (frame, fx, fy, tx, ty, color, sw, mode);
}

/** Get coordinates for point of perpendicular line within grid.
 * Translates from (x, y) into (&px, &py).  */
void get_perpendicular_line_point(int x, int y, int * px, int * py, int gridWidth, int gradientSign) 
{
   bool isNegative = gradientSign < 0;

   int lx = isNegative * (gridWidth - 1) + gradientSign * x;
   int ly = y * gradientSign;

   *px = ly;
   *py = gridWidth - 1 - lx;
}

/** Make sure, that min and max are in the right order */
inline void fix_min_max(int * min, int * max) 
{
   if (*min > *max) { 
      int tmp = *min;
      *min = *max;
      *max = tmp;
   }
}

/** Get list filled with the x-values of the perpendicular line to the line defined by (fx, tx). 
 * @returns Reference to static vector. */
std::vector<int> & get_stroke_pattern(int fx, int fy, int tx, int ty, int patternHeight, int gradientSign) 
{
   static std::vector<int> s_strokePattern; // container for current stroke pattern

   s_strokePattern.resize(patternHeight, NONE); // set the size of the container to relevant size

   // center point
   int cx; // to be found
   int cy = patternHeight / 2;   

   bresenham_line(0, 0, tx - fx, ty - fy, [&](int x, int y) -> bool {
      if (x >= patternHeight) return false; // if we have evaluated enough, stop the algorithm

      int px, py;
      get_perpendicular_line_point(x, y, &px, &py, patternHeight, gradientSign);

      s_strokePattern[py] = px;

      if (py == cy) 
         cx = px; // set middle x-value

      return true;
   });

   // move every bit of the perpendicular line grid, 
   // so that the middle pixel of the line is in the center
   int perpLineOffset = - cx; // the amount to offset every pixel
   for (int y = 0; y < s_strokePattern.size(); y++)
      s_strokePattern[y] += perpLineOffset;

   return s_strokePattern;
}

/** Limit the length of a given stroke pattern of a line to the given strokeweight.
 * @returns Reference to static vector. */
std::vector<int> & limit_stroke_pattern_to_stroke_weight(const std::vector<int> & strokePattern, int sw) 
{
   static std::vector<int> s_limitedStrokePattern;
   s_limitedStrokePattern.resize(strokePattern.size(), NONE);

   // copy stroke pattern
   std::copy(strokePattern.begin(), strokePattern.end(), s_limitedStrokePattern.begin());

   int minX = s_limitedStrokePattern.front();
   int maxX = s_limitedStrokePattern.back();

   // sort min and max values
   fix_min_max(&minX, &maxX);

   // calculate gradient of line
   int lineX = maxX - minX;
   float gradient = static_cast<float> (s_limitedStrokePattern.size()) / static_cast<float> (lineX);

   // compute appropriate grid height based on the line reach X
   int lineY = static_cast<int> (std::round(std::sin(std::atan(gradient)) * float(sw))); // sin(atan(gradient)) * strokeweight

   // remove top and bottom lines according to gradient
   int n_linesToRemove = s_limitedStrokePattern.size() - lineY;

   for (int i = 0; i < n_linesToRemove; i++) {
      bool isEven = i % 2 == 0;
      int y = i / 2;
      if (isEven)
         s_limitedStrokePattern[s_limitedStrokePattern.size() - 1 - y] = NONE; // remove from bottom
      else
         s_limitedStrokePattern[y] = NONE; // remove from top
   }

   return s_limitedStrokePattern;
}

/** Find gaps in given stroke pattern and return a stroke pattern of the necessary points to fill the gaps,
 * when the y-direction changes in the bresenham line algorithm.
  * @returns Reference to a static vector. */
std::vector<int> & get_gaps_from_stroke_pattern(const std::vector<int> & strokePattern, int gradientSign)
{
   static std::vector<int> s_gapStrokePattern;
   s_gapStrokePattern.resize(strokePattern.size(), NONE);

   // compare strokePattern to itself, but shifted as if the y-direction changed
   for (int y = 0; y < strokePattern.size(); y++) {
      int x = strokePattern[y];

      // if the stroke pattern is undefined for the given y-line
      if (x == NONE) {
         s_gapStrokePattern[y] = NONE;
         continue; 
      }

      // y = y + yDir
      int shiftedY = y + gradientSign;

      // if shiftedY is off-bounds
      if (shiftedY < 0 || shiftedY >= strokePattern.size()) { 
         s_gapStrokePattern[y] = NONE;
         continue; 
      }

      // x = x - 1
      int shiftedX = strokePattern[shiftedY] - 1;

      // check for |X|gap|X|
      if (shiftedX + 2 == x)
         s_gapStrokePattern[y] = shiftedX + 1;
      else
         s_gapStrokePattern[y] = NONE;		
   }

   return s_gapStrokePattern;
}

/** Create a stroke pattern of a circle for ends of lines with the given strokeweight
 *  @returns Reference to static s_circleStrokePattern vector. */
std::vector<int> & get_circle_stroke_pattern(int patternHeight) 
{
   static std::vector<int> s_circleStrokePattern;
   s_circleStrokePattern.resize(patternHeight, NONE);

   int r = patternHeight / 2;
   bresenham_circle(r, [&](int circx, int circy) -> void {
      // y-values for the 4 quadrants in the positive-x side.
      int y1 = r + circy, y2 = r - circy, 
          y3 = r + circx, y4 = r - circx;

      s_circleStrokePattern[y1] = circx;
      s_circleStrokePattern[y2] = circx;
      s_circleStrokePattern[y3] = circy;
      s_circleStrokePattern[y4] = circy;
   });

   return s_circleStrokePattern;
}

/** Draw the given stroke pattern to the (cx, cy) coordinate. */
void draw_stroke_pattern(const std::vector<int> & strokePattern, int cx, int cy, std::function<void(int x, int y)> setpx) 
{
   int sz = strokePattern.size();
   for (int y = 0; y < sz; y++) {
      if (strokePattern[y] == NONE) continue;

      setpx(cx + strokePattern[y], cy + y - sz / 2);
   }
}

/** Shift the given stroke pattern values by the given (xOff, yOff) vector. 
 * Note: If stroke-pattern is shifted out of y-range, elements are deleted, as the size of the stroke pattern is unaltered. */
void shift_stroke_pattern(std::vector<int> & strokePattern, int xOff, int yOff) 
{
   // if we shift negative y, go top down
   if (yOff < 0) {
      for (int y = 0; y < strokePattern.size(); y++) {
         if (y - yOff < 0 || y - yOff >= strokePattern.size() || strokePattern[y - yOff] == NONE)
            strokePattern[y] = NONE;
         else 
            strokePattern[y] = strokePattern[y - yOff] + xOff;
      }
   // if we shift positive y, go bottom up
   } else {
      for (int y = strokePattern.size() - 1; y >=0; y--) {
         if (y - yOff < 0 || y - yOff >= strokePattern.size() || strokePattern[y - yOff] == NONE)
            strokePattern[y] = NONE;
         else 
            strokePattern[y] = strokePattern[y - yOff] + xOff;
      }
   }
} 

/** Return a vector, that is the result of shifting the given lineStroke by (xOff, yOff), and removing any x-coordinates, that are outside the bounds of the given circleStroke. */
std::vector<int> & shift_and_limit_line_to_circle_stroke(const std::vector<int> & lineStroke, const std::vector<int> & circleStroke, int xOffset, int yOffset) 
{
   static std::vector<int> s_resultStroke;
   s_resultStroke.resize(lineStroke.size(), NONE);

   // copy lineStroke into s_resultStroke
   std::copy(lineStroke.begin(), lineStroke.end(), s_resultStroke.begin());

   shift_stroke_pattern(s_resultStroke, xOffset, yOffset);

   // go through stroke pattern, and remove any x-values, that are outside the range of the circle
   for (int y = 0; y < s_resultStroke.size() ; y++) {
      int lx = s_resultStroke[y];

      if (lx == NONE) continue; // line undefined


      int rBound =  circleStroke[y];
      int lBound = -circleStroke[y];

      if (lx >= lBound && lx <= rBound) 
         s_resultStroke[y] = lx;
      else
         s_resultStroke[y] = NONE;
   }

   shift_stroke_pattern(s_resultStroke, -xOffset, -yOffset);

   return s_resultStroke;
}


/** Draw a thick line, that has an absolute gradient less than one (abs(grad) < 1).
 * @param setpx: User-defined function for drawing a pixel at (x, y).
 * @param sethorisontal: User-defined function for drawing a line from (fx, y) -> (tx, y). */
void draw_stroke_line_low(int fx, int fy, int tx, int ty, int sw, LineMode mode, std::function<void(int x, int y)> setpx, std::function<void(int x, int fy, int ty)> sethorisontal) 
{
   // set the pattern to the maximum height -> the line, if it was vertical
   int patternHeight = sw; 

   int gradientSign = (ty - fy >= 0) * 2 - 1;
   // these are all pointers to static resources 
   const std::vector<int> & lineStrokePattern   = get_stroke_pattern(fx, fy, tx, ty, patternHeight, gradientSign);
         std::vector<int> & limitedLine         = limit_stroke_pattern_to_stroke_weight(lineStrokePattern, sw);
         std::vector<int> & gapPoints           = get_gaps_from_stroke_pattern(limitedLine, gradientSign);
   const std::vector<int> & circleStrokePattern = get_circle_stroke_pattern(patternHeight);


   // calculate the offsets from the edge of the line 
   // to the center of the circle
   float gradient = std::atan(float(ty - fy) / float(tx - fx));       // a = tan^-1(dy / dx)
   int xOffset    = std::cos(gradient) * float(patternHeight) * 0.5f; // a = cos(A) * c
   int yOffset    = std::sin(gradient) * float(patternHeight) * 0.5f; // a = cos(A) * c

   // the edges of the circle -> where the algorithm has to step from and to
   int algoFromX, algoFromY, algoToX, algoToY;
   
   if (mode == LineMode::SMOOTH || mode == LineMode::ROUGHLONG) {
      algoFromX = fx - xOffset;
      algoFromY = fy - yOffset;
      algoToX   = tx + xOffset;
      algoToY   = ty + yOffset;
   } else if (mode == LineMode::ROUGH)  {
      algoFromX = fx; algoToX = tx;
      algoFromY = fy; algoToY = ty;
   }
   
   // the circle has its centre at the end points
   // only actually used, if mode == LineMode::SMOOTH
   int staCircX = fx;
   int staCircY = fy;
   int endCircX = tx;
   int endCircY = ty;

   bool rough = mode == LineMode::ROUGH || mode == LineMode::ROUGHLONG;

   int previousY = fy; 
   // start algorithm (steps through x)
   bresenham_line(algoFromX, algoFromY, algoToX, algoToY, [&](int x, int y) -> bool {
      if (rough || (x > staCircX && x < endCircX)) {
         // straight part of the line

         draw_stroke_pattern(limitedLine, x, y, setpx);

         // if we stepped down fill the gaps
         if (y != previousY) 
            draw_stroke_pattern(gapPoints, x, y, setpx);

      } else if (x <= staCircX) { // draw start circle
         // the amount to shift the line
         int dx = x - staCircX;
         int dy = y - staCircY;  

         // shift the limitedLine pattern and restrict it within the boundaries of the circle
         const std::vector<int> & shiftedPattern = shift_and_limit_line_to_circle_stroke(limitedLine, circleStrokePattern, dx, dy);
         draw_stroke_pattern(shiftedPattern, x, y, setpx);

         if (y != previousY) {
            const std::vector<int> & shiftedGaps = shift_and_limit_line_to_circle_stroke(gapPoints, circleStrokePattern, dx, dy);

            draw_stroke_pattern(shiftedGaps, x, y, setpx);
         }
      } else { // draw end circle
         // same as above
         int dx = x - endCircX;
         int dy = y - endCircY;
         
         const std::vector<int> & shiftedPattern = shift_and_limit_line_to_circle_stroke(limitedLine, circleStrokePattern, dx, dy);
         draw_stroke_pattern(shiftedPattern, x, y, setpx);

         if (y != previousY) {
            const std::vector<int> & gapInsidePattern = shift_and_limit_line_to_circle_stroke(gapPoints, circleStrokePattern, dx, dy);
            
            draw_stroke_pattern(gapInsidePattern, x, y, setpx);
         }
      }

      previousY = y;
      return true;
   });
}

/** Draw a line from (fx, fy) to (tx, ty), that has a gradient less than 1. */ 
void draw_stroke_low(Frame & frame, int fx, int fy, int tx, int ty, Rgba color, int sw, LineMode mode) 
{
   std::function<void(int, int)> setpx = [&](int x, int y) -> void {
      frame.set_pixel(x, y, color);   
   };
   
   std::function<void(int, int, int)> sethorisontal = [&](int fx, int tx, int y) -> void {
      set_horisontal_line(frame, fx, tx, y, color);
   };

   draw_stroke_line_low(fx, fy, tx, ty, sw, mode, setpx, sethorisontal);
}

/** Draw a line from (fx, fy) to (tx, ty), that has a gradient greater than 1. */ 
void draw_stroke_hi(Frame & frame, int fx, int fy, int tx, int ty, Rgba color, int sw, LineMode mode) 
{
   // use the draw_stroke_line_low, but switch x with y in the drawing functions.
   std::function<void(int, int)> setpx = [&](int x, int y) -> void {
      frame.set_pixel(y, x, color);   
   };
   
   std::function<void(int, int, int)> sethorisontal = [&](int fx, int tx, int y) -> void {
      for (int x = fx; x <= tx; x++)
         frame.set_pixel(y, x, color);
   };

   draw_stroke_line_low(fy, fx, ty, tx, sw, mode, setpx, sethorisontal);
}

#include "drawcircle.hpp"
#include "drawpixel.hpp"
#include "../../frame.hpp"
#include "bresenham.hpp"

#include <array>
#include <vector>

using namespace fbg;

constexpr float sin45 = 0.7071067811865f;

// Octants ((x, y) is normally in the "1" octant):
//			  |            |      
//      7 -|- 6         |   
// 	/    |    \       |          
//   3     |     2      |      
// --|-----+-----|- +x  |
//   1     |     0      |      
// 	\	  |    /       |   
//      5 -|- 4         |   
//  		  |            |
// 		  +y           |            

/** Get octant points for given reference point and x-y value pair (referring to the first octant). */
void get_octants(std::array<int, 8> & octantsX, std::array<int, 8> & octantsY, int cx, int cy, int x, int y) 
{
   octantsX[0] = cx + x; octantsY[0] = cy + y;
   octantsX[1] = cx - x; octantsY[1] = cy + y; 
   octantsX[2] = cx + x; octantsY[2] = cy - y;
   octantsX[3] = cx - x; octantsY[3] = cy - y; 
   octantsX[4] = cx + y; octantsY[4] = cy + x; 
   octantsX[5] = cx - y; octantsY[5] = cy + x; 
   octantsX[6] = cx + y; octantsY[6] = cy - x; 
   octantsX[7] = cx - y; octantsY[7] = cy - x; 
}

constexpr int NONE = -1;

void compute_circle_stroke_single(Frame & frame, int cx, int cy, int r, Rgba color) 
{
   std::array<int, 8> ocx;
   std::array<int, 8> ocy;

   bresenham_circle(r, [&](int x, int y) -> void {
      get_octants(ocx, ocy, cx, cy, x, y);

      bool yIsNot0 = y != 0;

      // draw octants 0 and 1
      frame.set_pixel(ocx[0], ocy[0], color);
      frame.set_pixel(ocx[1], ocy[1], color);

      // draw octants 2 and 3, but avoid overlap with 0 and 1
      if (yIsNot0) {
         frame.set_pixel(ocx[2], ocy[2], color);
         frame.set_pixel(ocx[3], ocy[3], color);
      }

      if (y != x) {
         // draw octants 4 and 6
         frame.set_pixel(ocx[4], ocy[4], color); 
         frame.set_pixel(ocx[6], ocy[6], color); 

         // draw octants 5 and 7, but dont draw the
         // point they share.
         if (yIsNot0) {
            frame.set_pixel(ocx[5], ocy[5], color); 
            frame.set_pixel(ocx[7], ocy[7], color); 
         } 
      }
   });
}

void double_octant_stroke_to_quartant(std::vector<int> & strokePattern) 
{
   int maxX = strokePattern[0] + 1; // equal to radius
   // set the vector to be able to hold all the y-values.
   strokePattern.resize(maxX, NONE); 

   // copy all points from the first octant to the joined octant
   // x becomes y, and y becomes x
   for (size_t y = 0; y < strokePattern.size(); y++)
      strokePattern[strokePattern[y]] = y;
}

void compute_circle_stroke_multi(Frame & frame, int cx, int cy, int r, Rgba color, int sw) 
{
   int lOff, rOff;
   offsets_from_strokeweight(sw, &lOff, &rOff);

   std::vector<int> innerCircleStroke, outerCircleStroke;
   bresenham_circle(r + rOff, [&](int x, int y) -> void {
      // fill up array with points
      outerCircleStroke.push_back(x); 
   });

   bresenham_circle(r - lOff, [&](int x, int y) -> void {
      innerCircleStroke.push_back(x); // starts at x = 0, and fills up the 0'th octant
   });

   double_octant_stroke_to_quartant(outerCircleStroke);
   double_octant_stroke_to_quartant(innerCircleStroke);
   
   innerCircleStroke.resize(outerCircleStroke.size(), NONE);

   for (size_t y = 0; y < outerCircleStroke.size(); y++) {
      int innerX = innerCircleStroke[y];
      int outerX = outerCircleStroke[y];

      if (innerX == NONE) {
         set_horisontal_line(frame, cx - outerCircleStroke[y], cx + outerCircleStroke[y], cy + y, color);
         if (y != 0)
            set_horisontal_line(frame, cx - outerCircleStroke[y], cx + outerCircleStroke[y], cy - y, color);

         continue;
      }

      set_horisontal_line(frame, cx + innerCircleStroke[y], cx + outerCircleStroke[y], cy + y, color);
      set_horisontal_line(frame, cx - outerCircleStroke[y], cx - innerCircleStroke[y], cy + y, color);

      if (y != 0) {
         set_horisontal_line(frame, cx + innerCircleStroke[y], cx + outerCircleStroke[y], cy - y, color);
         set_horisontal_line(frame, cx - outerCircleStroke[y], cx - innerCircleStroke[y], cy - y, color);
      }
   }
}

/** Draw pixels of given circle, defined by center point (cx, cy) and radius r, to the given frame.
 * Pre-condition: Strokeweight is less than half of the radius.  */
void fbg::compute_circle_stroke(Frame &frame, int cx, int cy, int r, Rgba color, int sw) 
{
   if (sw * 2 >= r) throw std::runtime_error("Cannot draw stroke of circle, where 2 * strokeweight is greater than radius.");

   if (sw <= 1) compute_circle_stroke_single(frame, cx, cy, r, color);
   else         compute_circle_stroke_multi (frame, cx, cy, r, color, sw);
}

/** Draw pixels of given circles fill to the given frame. */
void fbg::compute_circle_fill(Frame &frame, int cx, int cy, int r, Rgba color) 
{
   float fr = static_cast<float> (r);
   int posLimitY = (sin45 * fr);
   int negLimitY = (0.0f - sin45 * fr);

   bresenham_circle(r, [&](int x, int y) -> void {
      bool yIsNot0 = y != 0;

      // draw lines horisontally
      if ( y < posLimitY) 
         set_horisontal_line(frame, cx - x + 1, cx + x - 1, cy + y, color); // 1 -> 0
      
      if (-y > negLimitY && yIsNot0) // avoid painting line y = 0 twice
         set_horisontal_line(frame, cx - x + 1, cx + x - 1, cy - y, color); // 4 -> 3
      
      set_vertical_line(frame, cx + y, cy + posLimitY, cy + x - 1, color);
      if (yIsNot0) // avoid paint vertical line x == 0 twice
         set_vertical_line(frame, cx - y, cy + posLimitY, cy + x - 1, color);

      set_vertical_line(frame, cx + y, cy - x + 1, cy + negLimitY, color);
      if (yIsNot0)
         set_vertical_line(frame, cx - y, cy - x + 1, cy + negLimitY, color);
   });
}
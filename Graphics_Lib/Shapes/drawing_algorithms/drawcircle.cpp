#include "drawcircle.hpp"
#include "drawpixel.hpp"
#include "../../frame.hpp"
#include "bresenham.hpp"

using namespace fbg;

/* Draw a point (x, y) from the first octant to every octant, with (cx, cy) as a referencepoint. */
void draw_8_octants(Frame & frame, int cx, int cy, int x, int y, const Rgba & color, int sw) {
   frame.set_pixel(cx + x, cy + y, color); // Octants ((x, y) is normally in the "1" octant):
   frame.set_pixel(cx - x, cy + y, color); //			|
   frame.set_pixel(cx + x, cy - y, color); //       8 | 7
   frame.set_pixel(cx - x, cy - y, color); //   4     |     3
   frame.set_pixel(cx + y, cy + x, color); // --------+------- +x
   frame.set_pixel(cx - y, cy + x, color); //   2     |     1
   frame.set_pixel(cx + y, cy - x, color); //       6 | 5
   frame.set_pixel(cx - y, cy - x, color); //  		   |
                                           // 		  +y
}

/** Draw lines of circle based on a point from the */

void draw_circle_line_from_octant_point(Frame &frame, int cx, int cy, int x, int y, const Rgba &color){
   // Octants ((x, y) is normally in the "1" octant):
   //			  |
   //       8 | 7
   // 		  |
   //   4     |     3
   // --------+------- +x
   //   2     |     1
   // 		  |
   //       6 | 5
   //  		  |
   // 		  +y

   // draw lines horisontally
   //                             from x       to x         y
   set_horisontal_line(frame, cx - x + 1, cx + x - 1, cy + y, color); // 2 -> 1
   set_horisontal_line(frame, cx - x + 1, cx + x - 1, cy - y, color); // 4 -> 3
   set_horisontal_line(frame, cx - y + 1, cx + y - 1, cy + x, color); // 6 -> 5
   set_horisontal_line(frame, cx - y + 1, cx + y - 1, cy - x, color); // 8 -> 7
}

/** Draw pixels of given circle, defined by center point (cx, cy) and radius r, to the given frame. */
void fbg::compute_circle_stroke(Frame &frame, int cx, int cy, int r, const Rgba &color, int sw) {
   int lOff, rOff;
   offsets_from_strokeweight(sw, &lOff, &rOff);

   bresenham_circle(r, [&](int x, int y) -> void {
      for (int i = x - lOff; i <= x + rOff; i++) // take stroke weight into account
         draw_8_octants(frame, cx + x, cy + y, i, y, color, sw);
   });
}

/** Draw pixels of given circles fill to the given frame. */
void fbg::compute_circle_fill(Frame &frame, int cx, int cy, int r, const Rgba &color) {
   bresenham_circle(r, [&](int x, int y) -> void {
      draw_circle_line_from_octant_point(frame, cx, cy, x, y, color);
   });
}
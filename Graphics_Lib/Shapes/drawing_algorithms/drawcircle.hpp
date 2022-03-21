#ifndef DRAW_CIRCLE_HPP
#define DRAW_CIRCLE_HPP

#include "../../frame.hpp"
#include "drawpixel.hpp"

// this file is only defined in headers, because it uses templates.

namespace fbg
{
   /* Draw a point (x, y) from the first octant to every octant, with (cx, cy) as a referencepoint. */
   template <typename Col>
   void draw_8_octants(Frame<Col> &frame, int cx, int cy, int x, int y, const Col &color, int sw) {
      set_pixel(frame, cx + x, cy + y, color); // Octants ((x, y) is normally in the "1" octant):
      set_pixel(frame, cx - x, cy + y, color); //			  |
      set_pixel(frame, cx + x, cy - y, color); //       8 | 7
      set_pixel(frame, cx - x, cy - y, color); //   4     |     3
      set_pixel(frame, cx + y, cy + x, color); // --------+------- +x
      set_pixel(frame, cx - y, cy + x, color); //   2     |     1
      set_pixel(frame, cx + y, cy - x, color); //       6 | 5
      set_pixel(frame, cx - y, cy - x, color); //  		  |
                                               // 		  +y
   }

   /** Draw lines of circle based on a point from the */
   template <typename Col>
   void draw_circle_line_from_octant_point(Frame<Col> &frame, int cx, int cy, int x, int y, const Col &color){
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
      compute_horisontal_line(frame, cx - x + 1, cy + y, cx + x - 1, cy + y, color); // 2 -> 1
      compute_horisontal_line(frame, cx - x + 1, cy - y, cx + x - 1, cy - y, color); // 4 -> 3
      compute_horisontal_line(frame, cx - y + 1, cy + x, cx + y - 1, cy + x, color); // 6 -> 5
      compute_horisontal_line(frame, cx - y + 1, cy - x, cx + y - 1, cy - x, color); // 8 -> 7
   }

   /** Draw pixels of given circle, defined by center point (cx, cy) and radius r, to the given frame. */
   template <typename Col>
   void compute_circle_stroke(Frame<Col> &frame, int cx, int cy, int r, const Col &color, int sw) {
      int x = r;
      int y = 0;

      int D = 3 - 2 * r;

      int lOff, rOff;
      offsets_from_strokeweight(sw, &lOff, &rOff);

      while (x >= y)
      {
         for (int i = x - lOff; i <= x + rOff; i++) // take stroke weight into account
            draw_8_octants(frame, cx, cy, i, y, color, sw);

         if (D > 0)
         {
            D = D + 4 * (y - x) + 10;
            x = x - 1;
         }
         else // D <= 0:
            D = D + 4 * y + 6;

         y = y + 1;
      }
   }

   /*
      Draw pixels of given circles fill to the given frame
   */
   template <typename Col>
   void compute_circle_fill(Frame<Col> &frame, int cx, int cy, int r, const Col &color) {
      int x = r;
      int y = 0;
      int D = 3 - 2 * r;

      while (x >= y)
      {
         draw_circle_line_from_octant_point(frame, cx, cy, x, y, color);
         if (D > 0)
         {
            D = D + 4 * (y - x) + 10;
            x = x - 1;
         }
         else // D <= 0:
            D = D + 4 * y + 6;

         y = y + 1;
      }
   }
};
#endif
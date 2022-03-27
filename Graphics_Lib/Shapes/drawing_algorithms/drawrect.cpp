#include "drawrect.hpp"
#include "drawpixel.hpp"

using namespace fbg;

/*
   Compute the vertical pixels for a given pixel with the correct stroke
*/
void draw_stroke_part_vertical(Frame & frame, int x, int y, const Rgba & color, int sw) {
   int lOff, rOff;
   offsets_from_strokeweight(sw, &lOff, &rOff);

   int fy = y - lOff;
   int ty = y + rOff;

   for (int py = fy; py <= ty; py++)
      frame.set_pixel(x, py, color);
}

/** Compute the horisontal pixels for a given pixel with the correct stroke. */
void draw_stroke_part_horisontal(Frame & frame, int x, int y, const Rgba & color, int sw) {
   int lOff, rOff;
   offsets_from_strokeweight(sw, &lOff, &rOff);
   int fx = x - lOff; 
   int tx = x + rOff; // from x and to x
   
   set_horisontal_line(frame, fx, tx, y, color);
}

/** Draws stroke of axis aligned rectangle r on pixel grid. */
void fbg::compute_AA_rect_stroke(Frame & frame, int rx, int ry, int rw, int rh, const Rgba & color, int sw) {
   // take stroke width into account
   int lOff, rOff; // offsets created by strokeweight

   offsets_from_strokeweight(sw, &lOff, &rOff);

   int lOuterX = rx - lOff;      // left x
   int rOuterX = rx + rw + rOff; // right x

   int fy = ry - lOff;
   int ty = ry + rOff;

   // move from y to to y
   for (int y = fy; y <= ty; y++) {
      set_horisontal_line(frame, lOuterX, rOuterX, y, 		    color); // top line	
      set_horisontal_line(frame, lOuterX, rOuterX, y + rh - 1, color); // bottom line
   }

   // vertical lines
   int topY = ry - lOff;
   int botY = ry + rh - 1 + rOff;

   if (rx >= 0 && rx < frame.w) {
      for (int y = topY; y <= botY && y < frame.h; y++) {
         if (y < 0) continue; // if offscreen -> move to next pixel
         
         draw_stroke_part_horisontal(frame, rx, y, color, sw);
         draw_stroke_part_horisontal(frame, rx + rw, y, color, sw);
      }
   }
}

/** Compute fill of axis aligned rectangle defined by top point (rx, ry) and with width rw and height rh. */
void fbg::compute_AA_rect_fill(Frame & f, int rx, int ry, int rw, int rh, const Rgba & color) {
   // bottom right point
   const int brx = rx + rw;
   const int bry = ry + rh;

   // loop from top to bottom (only inside rect)
   for (int y = ry + 1; y <= bry - 1; y++) {
      set_horisontal_line(f, rx + 1, brx - 1, y, color);
   }
}
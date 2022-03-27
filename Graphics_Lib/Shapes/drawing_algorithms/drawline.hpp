#ifndef DRAW_LINE_HPP
#define DRAW_LINE_HPP

#include "../../rgba.hpp"
#include "../../frame.hpp"

namespace fbg {
   /** SMOOTH: Half-circles at the end of the line. ROUGH: Flat ends at the precise coordinates. ROUGHLONG: Flat ends at the coordinates, plus the strokeweight (perfect for rectangles) */
   enum LineMode { UNDEFINED, SMOOTH, ROUGH, ROUGHLONG }; 
   void compute_line_stroke(Frame & frame, int fx, int fy, int tx, int ty, const Rgba & color, int sw, LineMode mode);
};
#endif
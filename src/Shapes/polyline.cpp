#include "Shapes/polyline.hpp"
#include "Shapes/drawing_algorithms/drawline.hpp"
#include "Shapes/drawing_algorithms/drawpolyline.hpp"

using namespace fbg;

void fbg::Polyline::draw_stroke(Frame & frame) const {
   if (!m_doStroke) return;

   // Set from to the last vector
   V2d<int> to, from = get_point(0);

   // draw every line segment by looping through list of points
   for (int i = 1; i < num_points(); i++) 
   {
      to = get_point(i);
      compute_line_stroke(frame, from.x, from.y, to.x, to.y, stroke(), strokeweight(), LineMode::ROUGH); 
      from = to;
   }

   // if polyline is closed, draw the remaining line.
   if (!m_open) {
      from = get_point(int(num_points()) - 1);
      to = get_point(0);
      compute_line_stroke(frame, from.x, from.y, to.x, to.y, stroke(), strokeweight(), LineMode::ROUGH);
   }
}

void fbg::Polyline::draw_fill(Frame & frame) const 
{
   if (!m_doFill) return;

   fbg::compute_polyline_convex_fill(frame, get_points(), fill());
}
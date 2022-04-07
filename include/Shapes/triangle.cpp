#include "triangle.hpp"
#include "drawing_algorithms/drawline.hpp"
#include "drawing_algorithms/drawpolyline.hpp"

using namespace fbg;

void Triangle::draw_stroke(Frame & frame) 
{
   if (!m_doStroke) return;

   // Set from to the last vector
   V2d<int> to, from = get_point(0);

   // draw every line segment by looping through list of points
   for (int i = 1; i < num_points(); i++) {
      to = get_point(i);
      compute_line_stroke(frame, from.x, from.y, to.x, to.y, stroke(), strokeweight(), LineMode::ROUGH); 
      from = to;
   }

   to = get_point(0);
   compute_line_stroke(frame, from.x, from.y, to.x, to.y, stroke(), strokeweight(), LineMode::ROUGH); 
}

void Triangle::draw_fill(Frame & frame) 
{
   if (!m_doFill) return;

   compute_polyline_convex_fill(frame, get_points(), fill());
}
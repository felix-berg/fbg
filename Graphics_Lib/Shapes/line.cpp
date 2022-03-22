#include "line.hpp"
#include "../V2d.hpp"
#include "../rgba.hpp"
#include "drawing_algorithms/drawline.hpp"
#include "../alphacomposite.hpp"

#include <iostream>
#include <cmath>

using namespace fbg;

/** Whether to draw lines with smooth edges of lines or not. */
bool Line::SMOOTH_EDGES = true; 
// standard value

/** Change given pixelbuffer based on the pixels of the line. */
void Line::draw_stroke(Frame & frame) {
   if (!m_doStroke) return;

   V2d<float> f = from();
   V2d<float> t = to();
   
   bool smooth = false;
   
   if (m_edgeType == Line::EDGETYPE::UNDEFINED)
      smooth = Line::SMOOTH_EDGES;
   else 
      smooth = m_edgeType == Line::EDGETYPE::SMOOTH;

   if (smooth)
      compute_line_stroke_smooth(frame, f.x, f.y, t.x, t.y, stroke(), strokeweight());
   else
      compute_line_stroke(frame, f.x, f.y, t.x, t.y, stroke(), strokeweight());
}
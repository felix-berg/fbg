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
void Line::draw_stroke(Frame & f) {
   if (!m_doStroke) return;

   V2d<int> fp = from();
   V2d<int> tp = to();

   bool smooth = false;
   
   if (m_edgeType == Line::EDGETYPE::UNDEFINED)
      smooth = Line::SMOOTH_EDGES;
   else 
      smooth = m_edgeType == Line::EDGETYPE::SMOOTH;

   if (smooth)
      compute_line_stroke_smooth(f, fp.x, fp.y, tp.x, tp.y, stroke(), strokeweight());
   else
      compute_line_stroke(f, fp.x, fp.y, tp.x, tp.y, stroke(), strokeweight());
}
#include "Shapes/line.hpp"
#include "V2d.hpp"
#include "rgba.hpp"
#include "Shapes/drawing_algorithms/drawline.hpp"
#include "alphacomposite.hpp"

#include <iostream>
#include <cmath>

using namespace fbg;

LineMode Line::DEFAULT_LINEMODE = SMOOTH;
int Line::DEFAULT_STROKEWEIGHT = 5;

/** Change given pixelbuffer based on the pixels of the line. */
void Line::draw_stroke(Frame & frame) const 
{
   if (!m_doStroke) return;

   V2d<float> f = from();
   V2d<float> t = to();
   
   LineMode m;
   
   m = m_edgeType;

   compute_line_stroke(frame, f.x, f.y, t.x, t.y, stroke(), strokeweight(), m);
}
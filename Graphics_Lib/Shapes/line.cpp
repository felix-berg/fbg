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

int Line::DEFAULT_STROKEWEIGHT = 5;

/** Change given pixelbuffer based on the pixels of the line. */
void Line::draw_stroke(Frame & frame) 
{
   if (!m_doStroke) return;

   V2d<float> f = from();
   V2d<float> t = to();
   
   LineMode m;
   
   if (m_edgeType == LineMode::UNDEFINED)
      m = Line::SMOOTH_EDGES ? LineMode::SMOOTH : LineMode::ROUGH; 
   else 
      m = m_edgeType;

   compute_line_stroke(frame, f.x, f.y, t.x, t.y, stroke(), strokeweight(), m);
}
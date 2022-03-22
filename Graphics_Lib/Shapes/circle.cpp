#include "drawing_algorithms/drawcircle.hpp"
#include "circle.hpp"

using namespace fbg;

void Circle::draw_stroke(Frame & f) {
   if (!m_doStroke) return;
   
   V2d<int> p = pos();
   int r = static_cast<int>(radius());

   compute_circle_stroke(f, p.x, p.y, r, stroke(), strokeweight());
}

void Circle::draw_fill(Frame & f) {
   if (!m_doFill) return;

   V2d<int> p = pos();
   int r = static_cast<int>(radius());

   compute_circle_fill(f, p.x, p.y, r, fill());
}
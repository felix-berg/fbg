#include "rect.hpp"
#include "../alphacomposite.hpp"
#include "drawing_algorithms/drawrect.hpp"
#include "drawing_algorithms/drawline.hpp"

using namespace fbg;

// Rect::DrawMode Rect::MODE = CENTER;

V2d<float> middle_point_to_topleft(const V2d<float> & m, float w, float h, float a) {
   V2d<float> v {- w * 0.5f, - h * 0.5f};
   v.rotate(a);
   return m + v;
}

template <typename T>
struct Corners { V2d<T> tl, tr, bl, br; };

constexpr float halfpi = 1.57079632679489661923f;
constexpr float twopi  = 6.28318530717958647692f;

template<typename T>
Corners<T> middle_point_to_corners(const V2d<float> & m, float w, float h, float a) {
   Corners<T> res { };

   V2d<float> v {- w * 0.5f, - h * 0.5f};
   v.rotate(a);
   // probably inefficient
   res.tl = m + v; v.rotate(halfpi);
   res.tr = m + v; v.rotate(halfpi);
   res.br = m + v; v.rotate(halfpi);
   res.bl = m + v;

   return std::move(res);
}

V2d<float> topleft_to_middle_point(const V2d<float> & crn, float w, float h, float a) {
   return { };
}

void Rect::draw_stroke(Frame & frame) {
   if (!m_doStroke) return;

   if (std::fmod(m_angle, twopi) == 0.0f) {
      V2d<int> p = middle_point_to_topleft(pos(), width(), height(), m_angle);
      compute_AA_rect_stroke(frame, p.x, p.y, width(), height(), stroke(), strokeweight());
   } else {
      Corners<int> crs = middle_point_to_corners<int>(pos(), width(), height(), m_angle);
      compute_line_stroke(frame, crs.tl.x, crs.tl.y, crs.tr.x, crs.tr.y, stroke(), strokeweight(), true);
      compute_line_stroke(frame, crs.tr.x, crs.tr.y, crs.br.x, crs.br.y, stroke(), strokeweight(), true);
      compute_line_stroke(frame, crs.br.x, crs.br.y, crs.bl.x, crs.bl.y, stroke(), strokeweight(), true);
      compute_line_stroke(frame, crs.bl.x, crs.bl.y, crs.tl.x, crs.tl.y, stroke(), strokeweight(), true);
   }

}

void Rect::draw_fill(Frame & frame) {
   if (!m_doFill) return;

   if (m_angle == 0.0f) {
      V2d<int> p = middle_point_to_topleft(pos(), width(), height(), m_angle);
      compute_AA_rect_fill(frame, p.x, p.y, width(), height(), fill());
   } else {

   }
}

void fbg::Rect::rotate(float a, const V2d<float> & ref) {
   V2d<float> diff = pos() - ref;
   diff.rotate(a);
   pos(ref + diff);

   // compare the angle of a point on the rectangle before and after rotation
   V2d<float> verticalLine = V2d<float>{10.0f, 0.0f};
   V2d<float> rr = pos() + verticalLine;
   rr = rr - ref;
   rr.rotate(a);
   rr += ref;
   rr -= pos();

   m_angle += angle_between(rr, verticalLine);

   std::cout << m_angle << '\n';  
}
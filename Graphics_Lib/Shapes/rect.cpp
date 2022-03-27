#include "rect.hpp"
#include "../alphacomposite.hpp"
#include "drawing_algorithms/drawrect.hpp"
#include "drawing_algorithms/drawline.hpp"
#include "drawing_algorithms/drawpolyline.hpp"

using namespace fbg;

Rect::DrawMode Rect::MODE = CENTER;

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

   V2d<float> tlOff {-w * 0.5f, -h * 0.5f};
   V2d<float> trOff { w * 0.5f, -h * 0.5f};
   V2d<float> brOff { w * 0.5f,  h * 0.5f};
   V2d<float> blOff {-w * 0.5f,  h * 0.5f};

   tlOff.rotate(a);
   trOff.rotate(a);
   brOff.rotate(a);
   blOff.rotate(a);


   // probably inefficient
   res.tl = m + tlOff;
   res.tr = m + trOff;
   res.br = m + brOff;
   res.bl = m + blOff;

   return std::move(res);
}

V2d<float> topleft_to_middle_point(const V2d<float> & tl, float w, float h, float a) {
   V2d<float> v { w * 0.5f, h * 0.5f };
   v.rotate(a);
   return tl + v;
}

constexpr float aaThreshold = 0.000001f;

void Rect::draw_stroke(Frame & frame) {
   if (!m_doStroke) return;

   // if the rectangle is axis aligned
   float modAngle = std::fmod(m_angle, twopi);

   if (modAngle > aaThreshold && modAngle < -aaThreshold) {
      V2d<float> tl;
      
      if (Rect::MODE == Rect::DrawMode::CORNER) 
         tl = pos();
      else
         tl = middle_point_to_topleft(pos(), width(), height(), m_angle);

      compute_AA_rect_stroke(frame, tl.x, tl.y, width(), height(), stroke(), strokeweight());
   } else {
      // get the middle point
      V2d<float> mid;
      if (Rect::MODE == Rect::DrawMode::CENTER)
         mid = pos();
      else if (Rect::MODE == Rect::DrawMode::CORNER) 
         mid = topleft_to_middle_point(pos(), width(), height(), m_angle);
      else
         throw std::runtime_error("Drawing mode is not defined.");

      Corners<float> crs = middle_point_to_corners<float>(mid, width(), height(), m_angle);

      // avoid visual glitches by rounding to integers instead of straight flooring.
      int tlX = std::round(crs.tl.x); int tlY = std::round(crs.tl.y);
      int trX = std::round(crs.tr.x); int trY = std::round(crs.tr.y);
      int brX = std::round(crs.br.x); int brY = std::round(crs.br.y);
      int blX = std::round(crs.bl.x); int blY = std::round(crs.bl.y);

      // draw lines from corner to corner
      compute_line_stroke(frame, tlX, tlY, trX, trY, stroke(), strokeweight(), LineMode::ROUGHLONG);
      compute_line_stroke(frame, trX, trY, brX, brY, stroke(), strokeweight(), LineMode::ROUGHLONG);
      compute_line_stroke(frame, brX, brY, blX, blY, stroke(), strokeweight(), LineMode::ROUGHLONG);
      compute_line_stroke(frame, blX, blY, tlX, tlY, stroke(), strokeweight(), LineMode::ROUGHLONG);
   }

}

void Rect::draw_fill(Frame & frame) {
   if (!m_doFill) return;

   // if the rectangle is axis aligned
   float modAngle = std::fmod(m_angle, twopi);
   if (modAngle < aaThreshold && modAngle > -aaThreshold) {
      V2d<int> p;
      if (Rect::MODE == Rect::DrawMode::CENTER) {
         p = middle_point_to_topleft(pos(), width(), height(), m_angle);
      } else {
         p = pos();
      }

      compute_AA_rect_fill(frame, p.x, p.y, width(), height(), fill());
   } else {
      V2d<float> mid;
      if (Rect::MODE == Rect::DrawMode::CENTER)
         mid = pos();
      else if (Rect::MODE == Rect::DrawMode::CORNER) 
         mid = topleft_to_middle_point(pos(), width(), height(), m_angle);
      else
         throw std::runtime_error("Drawing mode is not defined.");

      Corners<float> crs = middle_point_to_corners<float>(mid, width(), height(), m_angle);
      std::vector<V2d<float>> cVec = { { std::round(crs.tl.x), std::round(crs.tl.y) }, { std::round(crs.tr.x), std::round(crs.tr.y) }, { std::round(crs.br.x), std::round(crs.br.y) }, { std::round(crs.bl.x), std::round(crs.bl.y) }};
      compute_polyline_convex_fill(frame, cVec, fill());
   }
}

void fbg::Rect::rotate(float a, const V2d<float> & ref) {
   // compare the angle of a point on the rectangle before and after rotation
   V2d<float> verticalLine = V2d<float>{10.0f, 10.0f};
   V2d<float> rotatedPoint = pos() + verticalLine;
   rotatedPoint = rotatedPoint - ref;
   rotatedPoint.rotate(a);
   rotatedPoint += ref;

   V2d<float> diff = pos() - ref;
   diff.rotate(a);
   pos(ref + diff);

   rotatedPoint -= pos();

   m_angle += angle_between(rotatedPoint, verticalLine);
}
#include "rect.hpp"
#include "drawing_algorithms/drawrect.hpp"
#include "drawing_algorithms/drawline.hpp"
#include "drawing_algorithms/drawpolyline.hpp"

using namespace fbg;

Rect::DrawMode Rect::MODE = Rect::DrawMode::CENTER;

/** Rotate given vector by 90 degrees. */
void rotatevec90(V2d<float> & vec) {
   float tmp = vec.x;
   vec.x = vec.y;
   vec.y = -tmp;
}

/** Get the top left point from given middle vector and unit vector of rectangle. */
V2d<float> get_top_left_from_middle(const V2d<float> & middle, V2d<float> unit, float width, float height) 
{
   unit.rotate(quarterPi + pi);
   unit.size((V2d<float> {width / 2.0f, height / 2.0f}).size());

   return middle + unit;
}

/** Get a rectcorners structure holding tl, tr, br and bl points for this rectangle. */
RectCorners Rect::get_corners() const 
{
   V2d<float> unit = get_point(1) - pos();
   V2d<float> topLeft;

   if (Rect::MODE == Rect::DrawMode::CENTER)
      topLeft = get_top_left_from_middle(pos(), unit, width(), height());
   else if (Rect::MODE == Rect::DrawMode::CORNER) 
      topLeft = pos();
   else
      throw std::runtime_error("Error in drawing rectangle: Unknown drawing mode.");

   V2d<float> widthVec = width() * unit;
   rotatevec90(unit);
   V2d<float> heightVec = height() * unit;

   V2d<float> topRight = topLeft + widthVec;
   V2d<float> botRight = topLeft + widthVec + heightVec;
   V2d<float> botLeft  = topLeft + heightVec;

   return {
      { static_cast<int>(std::round(topLeft.x)),  static_cast<int>(std::round(topLeft.y))  },
      { static_cast<int>(std::round(topRight.x)), static_cast<int>(std::round(topRight.y)) },
      { static_cast<int>(std::round(botRight.x)), static_cast<int>(std::round(botRight.y)) },
      { static_cast<int>(std::round(botLeft.x)),  static_cast<int>(std::round(botLeft.y))  }
   };
}

constexpr float aaThreshold = 0.000001f;
void Rect::draw_stroke(Frame & frame)
{
   if (!m_doStroke) return;

   float angle = (get_point(1) - pos()).angle();
   float modAngle = std::fmod(angle, twoPi);

   // if the rectangle is axis aligned
   if (modAngle > 0 + aaThreshold && modAngle < 0 - aaThreshold) {
      V2d<float> tl;
      if (Rect::MODE == Rect::DrawMode::CORNER)
         tl = pos();
      else if (Rect::MODE == Rect::DrawMode::CENTER)
         tl = get_top_left_from_middle(pos(), get_point(1) - pos(), width(), height());
      else
         throw std::runtime_error("Unknown Rect::DrawMode");
      
      compute_AA_rect_stroke(frame, tl.x, tl.y, width(), height(), stroke(), strokeweight());
   } else {
      RectCorners crs = get_corners();

      compute_line_stroke(frame, crs.tl.x, crs.tl.y, crs.tr.x, crs.tr.y, stroke(), strokeweight(), LineMode::ROUGHLONG);
      compute_line_stroke(frame, crs.tr.x, crs.tr.y, crs.br.x, crs.br.y, stroke(), strokeweight(), LineMode::ROUGHLONG);
      compute_line_stroke(frame, crs.br.x, crs.br.y, crs.bl.x, crs.bl.y, stroke(), strokeweight(), LineMode::ROUGHLONG);
      compute_line_stroke(frame, crs.bl.x, crs.bl.y, crs.tl.x, crs.tl.y, stroke(), strokeweight(), LineMode::ROUGHLONG);
   }
}

void Rect::draw_fill(Frame & frame) 
{
   if (!m_doFill) return;

   float angle = (get_point(1) - pos()).angle();
   float modAngle = std::fmod(angle, twoPi);

   if (modAngle < aaThreshold && modAngle > -aaThreshold) {
      V2d<int> tl;
      if (Rect::MODE == Rect::DrawMode::CENTER) {
         tl = get_top_left_from_middle(pos(), get_point(1) - pos(), width(), height());
      } else {
         tl = pos();
      }

      compute_AA_rect_fill(frame, tl.x, tl.y, width(), height(), fill());
   } else {

      RectCorners crs = get_corners();

      std::vector<V2d<float>> crsVec { crs.tl, crs.tr, crs.br, crs.bl };
      compute_polyline_convex_fill(frame, crsVec, fill());
   }
}
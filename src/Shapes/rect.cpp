#include "Shapes/rect.hpp"
#include "Shapes/drawing_algorithms/drawrect.hpp"
#include "Shapes/drawing_algorithms/drawline.hpp"
#include "Shapes/drawing_algorithms/drawpolyline.hpp"

#include <cmath>

using namespace fbg;

Rect::DrawMode Rect::MODE = Rect::DrawMode::CENTER;

/** Rotate given vector by 90 degrees. */
void rotatevec90(V2d<float> & vec) {
   float tmp = vec.x;
   vec.x = -vec.y;
   vec.y = tmp;
}

void width_and_height_vectors(V2d<float> & widthVec, V2d<float> & heightVec, const V2d<float> & unit, float width, float height)
{
   widthVec  = unit * width;
   heightVec = unit * height;
   rotatevec90(heightVec);
}

/** Get the top left point from given middle vector and unit vector of rectangle. */
V2d<float> get_top_left_from_middle(const V2d<float> & middle, const V2d<float> & widthVec, const V2d<float> & heightVec) 
{
   return middle - widthVec * 0.5f - heightVec * 0.5f;
}

/** Get a rectcorners structure holding tl, tr, br and bl points for this rectangle. */
RectCorners Rect::get_corners() const 
{
   V2d<float> topLeft { };
   V2d<float> widthVec, heightVec;
   V2d<float> unit = get_point(1) - pos();
   width_and_height_vectors(widthVec, heightVec, unit, width(), height());

   if (Rect::MODE == Rect::DrawMode::CENTER) {

      topLeft = get_top_left_from_middle(pos(), widthVec, heightVec);
   }
   else if (Rect::MODE == Rect::DrawMode::CORNER) 
      topLeft = pos();
   else
      throw std::runtime_error("Error in drawing rectangle: Unknown drawing mode.");


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

constexpr float aaThreshold = 0.0001f;
void Rect::draw_stroke(Frame & frame) const
{
   if (!m_doStroke) return;

   float angle = (get_point(1) - pos()).angle();
   float modAngle = std::fmod(angle, twoPi);

   // if the rectangle is axis aligned
   if (modAngle > -aaThreshold && modAngle < aaThreshold) {
      V2d<float> tl;
      if (Rect::MODE == Rect::DrawMode::CENTER) {
         V2d<float> unit = get_point(1) - pos();
         V2d<float> widthVec, heightVec;
         width_and_height_vectors(widthVec, heightVec, unit, width(), height());

         tl = get_top_left_from_middle(pos(), widthVec, heightVec);
      }
      else
         tl = pos();
         
      compute_AA_rect_stroke(frame, tl.x, tl.y, width(), height(), stroke(), strokeweight());
   } else {
      RectCorners crs = get_corners();

      compute_line_stroke(frame, crs.tl.x, crs.tl.y, crs.tr.x, crs.tr.y, stroke(), strokeweight(), LineMode::ROUGHLONG);
      compute_line_stroke(frame, crs.tr.x, crs.tr.y, crs.br.x, crs.br.y, stroke(), strokeweight(), LineMode::ROUGHLONG);
      compute_line_stroke(frame, crs.br.x, crs.br.y, crs.bl.x, crs.bl.y, stroke(), strokeweight(), LineMode::ROUGHLONG);
      compute_line_stroke(frame, crs.bl.x, crs.bl.y, crs.tl.x, crs.tl.y, stroke(), strokeweight(), LineMode::ROUGHLONG);
   }
}

void Rect::draw_fill(Frame & frame) const 
{
   if (!m_doFill) return;

   float angle = (get_point(1) - pos()).angle();
   float modAngle = std::fmod(angle, twoPi);

   if (modAngle < aaThreshold && modAngle > -aaThreshold) {
      V2d<float> tl;
      if (Rect::MODE == Rect::DrawMode::CENTER) {
         V2d<float> unit = get_point(1) - pos();
         V2d<float> widthVec, heightVec;
         width_and_height_vectors(widthVec, heightVec, unit, width(), height());

         tl = get_top_left_from_middle(pos(), widthVec, heightVec);
      } else
         tl = pos();

      compute_AA_rect_fill(frame, tl.x, tl.y, width(), height(), fill());
   } else {

      RectCorners crs = get_corners();

      std::vector<V2d<float>> crsVec { crs.tl, crs.tr, crs.br, crs.bl };
      compute_polyline_convex_fill(frame, crsVec, fill());
   }
}
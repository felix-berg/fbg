#include "shape.hpp"
#include "BMP/bmp.hpp"

namespace fbg {
   class Image : public Shape {
   public:
      Image(const V2d<float> & p, const std::string & filename)
         : m_filename { filename }, m_frame { create_frame(filename) },
          m_w { static_cast<float> (m_frame.w) }, 
          m_h { static_cast<float> (m_frame.h) }
      { 
         add_point(p);
      }

      Image(float x, float y, const std::string & filename) 
         : Image { {x, y}, filename } { };

      Image(const std::string & filename)
         : Image { {0, 0}, filename } { };

      void pos(const V2d<float> & p) { set_point(0, p); };
      void pos(float x, float y)     { pos({x, y});     };

      V2d<float> pos() const { return get_point(0); };

   protected:
      void draw_stroke(Frame &) { };
      void draw_fill(Frame &) { };

   private:
      Frame m_frame;
      std::string m_filename;

      float m_w = 0.0f;
      float m_h = 0.0f;

      // privatise methods for drawing colors
      using Shape::fill;
      using Shape::stroke;
      using Shape::strokeweight;
      using Shape::noStroke;
      using Shape::noFill;
   };
}
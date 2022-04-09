#ifndef IMAGE_HPP
#define IMAGE_HPP

#include "shape.hpp"
#include "../frame.hpp"
#include "BMP/bmp.hpp"

namespace fbg {
   class Image : public Shape {
   public:
      Image(const V2d<float> & p, const std::string & filename);
      Image(float x, float y, const std::string & filename) 
         : Image { {x, y}, filename } { };

      Image(const std::string & filename)
         : Image { {0, 0}, filename } { };

      void pos(const V2d<float> & p) { set_point(0, p); };
      void pos(float x, float y)     { pos({x, y});     };

      V2d<float> pos() const { return get_point(0); };

      void width(float w)  { m_w = w; }
      void height(float h) { m_h = h; }

      float width()  const { return m_w; };
      float height() const { return m_h; };

   protected:
      void draw_stroke(Frame &) const { };
      void draw_fill(Frame &) const;

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

#endif
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

   protected:
      void draw_stroke(Frame &) { };
      void draw_fill(Frame & frame);

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
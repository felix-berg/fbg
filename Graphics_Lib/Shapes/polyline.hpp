#ifndef POLYGON_HPP
#define POLYGON_HPP

#include "shape.hpp"

// TODO: Javadocs

namespace fbg {
   class Polygon : public Shape {
   public:
      Polygon(std::initializer_list<V2d<float>> l) : Shape { l } { 
         // set default value of stroke
         stroke(255);
         strokeweight(3);
      };

      Polygon() {
         stroke(255);
         strokeweight(3);
      }

      void vertex(const V2d<float> & v) { Shape::add_point(v); };
         
      void vertex(float x, float y) { Shape::add_point({x, y}); };

      void set_vertex(int i, const V2d<float> & v) { Shape::set_point(i, v); };

      void open()  { m_open = true; };
      void close() { m_open = false; };

   protected:
      void draw_stroke(Frame & f);
      void draw_fill(Frame & f) { };
   private:
      bool m_open = true;
   };
};

#endif
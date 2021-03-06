#include "polyline.hpp"

namespace fbg {
   class Triangle : public Shape {
   public:
      struct NotAPoint : std::runtime_error {
         NotAPoint(int i)
            : std::runtime_error { "Invalid point index: " + std::to_string(i) }
         { };
      };

      Triangle(const V2d<float> & p1, const V2d<float> & p2, const V2d<float> & p3)
         : Shape { p1, p2, p3 } 
      {
         stroke(0);
         fill(255);
      };

      Triangle() 
         : Triangle { { 0.0f, 0.0f }, { 0.0f, 0.0f }, { 0.0f, 0.0f } } { };

      void point(int i, const V2d<float> & p) 
      { 
         if (i > 2) 
            throw NotAPoint(i);

         set_point(i, p); 
      };

      void point(int i, float x, float y) 
      {
         set_point(i, { x, y });
      }

      V2d<float> point(int i) const { 
         if (i > 2) 
            throw NotAPoint(i);
         
         return get_point(i); 
      };

   protected:
      void draw_stroke(Frame &) const;
      void draw_fill(Frame &) const;
   };
};
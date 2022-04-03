#include "polyline.hpp"

namespace fbg {
   class Triangle : public Shape {
   public:
      Triangle(const V2d<float> & p1, const V2d<float> & p2, const V2d<float> & p3)
         : Shape { p1, p2, p3 } { };

      Triangle() 
         : Triangle { { 0.0f, 0.0f }, { 0.0f, 0.0f }, { 0.0f, 0.0f } } { };

      void point(int i, const V2d<float> & p) 
      { 
         if (i > 2) 
            throw std::runtime_error("Triangle cannot be assigned a point outside the range of 0-2.\n");

         set_point(i, p); 
      };

      void point(int i, float x, float y) 
      {
         set_point(i, { x, y });
      }

      V2d<float> point(int i) const { 
         if (i > 2) 
            throw std::runtime_error("Triangle: cannot get a point outside the range of 0-2.\n");
         return get_point(i); 
      };

   protected:
      void draw_stroke(Frame &);
      void draw_fill(Frame &);
   };
};
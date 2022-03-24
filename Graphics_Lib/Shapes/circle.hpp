#ifndef CIRCLE_HPP
#define CIRCLE_HPP

#include "shape.hpp"
#include "../V2d.hpp"
#include "../frame.hpp"

namespace fbg {
   /** Circle class. 
    * Defined by center point (x, y) and radius (r). 
    * @param pos(): Getting/setting of center point.
    * @param radius(): Getting/setting of radius. */ 
   class Circle : public Shape {
   public:
      /** Constructor for circle class.
       * @param p: Center point of constructed circle.
       * @param r: Radius of constructed circle. */
      Circle(const V2d<float> & p, float r) : Shape { { p } } { 
         radius(r);
      };

      /** Constructor for circle class.
       * @param x: x-value for center point.
       * @param y: y-value for center point.
       * @param r: Radius of circle. */		
      Circle(float x, float y, float r) : Circle { {x, y}, r} { };

      /** Default constructor for circle class. 
       * Inititializes circle to x = 0, y = 0, r = 0. */
      Circle() 
         : Circle({0, 0}, 1) { };

      /** @returns Center point of circle */
      V2d<float> pos() const { return get_point(0); };
      /** @returns Radius of circle */
      float radius()   const { return m_radius; };
      
      /** Set position of center point of circle.
       * @param p: Center point of circle. */
      void pos(V2d<float> p) 		{ set_point(0, p); };
      /** Set position of center point of circle.
       * @param x: x-value of center point.
       * @param y: y-value of center point. */
      void pos(float x, float y) { set_point(0, V2d<float> {x, y}); };
      /** Set radius of circle. 
       * @param r: Radius of circle. */
      void radius(float r) { 
         if (r < 0.0f)
            throw std::runtime_error("Radius of circle cannot be negative.");
         m_radius = r;
      };

   protected:
      void draw_stroke(Frame & f);
      void draw_fill(Frame & f);	

   private:
      float m_radius;
   };
};
#endif
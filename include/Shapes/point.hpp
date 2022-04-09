#ifndef POINT_HPP
#define POINT_HPP

#include "shape.hpp"
#include "drawing_algorithms/drawpixel.hpp"
namespace fbg {
   
   /** Point class.
    * @param pos(): Getting/setting of position. */
   class Point : public Shape {
   public:
      /** Constructor for point.
       * @param p: Position */
      Point(const V2d<float> & p) :
         Shape { p } 
      {
         stroke(255);
      };

      /** Constructor for point.
       * @param x: x-value for position 
       * @param y: y-value for position */
      Point(float x, float y) : Point { V2d<float> { x, y } } { };
      
      /** Default constructor for point.
       * Sets position to (0, 0) */
      Point() : Shape { V2d<float> {0.0f, 0.0f} } { };

      /** Getter for position. 
       * @returns Position of point. */
      V2d<float> pos() const { return get_point(0); };
      
      /** Setter for position. 
       * @param p: Position of point. */
      void pos(V2d<float> p)   { set_point(0, p); 	  };

      /** Setter for position. 
       * @param x: x-value for position.
       * @param y: y-value for position.*/
      void pos(float x, float y) { set_point(0, {x, y}); };

   protected:
      void draw_stroke(Frame & f) const 
      {
         if (!m_doStroke) return;
         V2d<int> p = pos();
         draw_pixel(f, p.x, p.y, stroke(), strokeweight());
      }

      void draw_fill(Frame &) const { };

      // protect functions about fill
      using Shape::fill;
      using Shape::noFill;
   };
};

#endif
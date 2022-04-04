#include "shape.hpp"

namespace fbg {
   struct RectCorners {
      V2d<int> tl, tr, br, bl;
   };

   class Rect : public Shape {
   public:
      enum DrawMode { CORNER, CENTER };
      static DrawMode MODE;

      /** Constructor for rectangle. 
       * @param f: Top-left point.
       * @param w: Width of rectangle.
       * @param h: Height of rectangle. */
      Rect(const V2d<float> & f, float w, float h)
      : m_w { w }, m_h { h } 
      {
         add_point(f);
         add_point(f + V2d<float> { 1.0f, 0.0f });

         stroke(0);
         fill(255);
         strokeweight(1);
      }

      /** Constructor for rectangle. 
       * @param x: x-value for top-left point.
       * @param y: y-value for top-left point.
       * @param w: Width of rectangle.
       * @param h: Height of rectangle.
      */
      Rect(float x, float y, float w, float h) 
         : Rect { {x, y}, w, h } { };

      /** Default constructor for rectangle.
       * Sets top-left point to (0, 0), width and height to 1. */
      Rect() : Rect { {0, 0}, 0.0f, 0.0f } { };

      /** Getter for top-left point. 
       * @returns Top-left point. */
      V2d<float> pos() const { return get_point(0); };
      
      /** Getter for the width of the rectangle. 
       * @returns Width of rectangle. */
      float width() const { return m_w; };

      /** Getter for the height of the rectangle.
       * @returns Height of rectangle */
      float height() const { return m_h; };
      
      /** Setter for top-left point.
       * @param p: Position for top-left point. */
      void pos(V2d<float> p) { 
         V2d<float> unitV { 1.0f, 0.0f };
         unitV.rotate(angle());

         set_point(0, p);
         set_point(1, p + unitV);
      };

      /** Setter for top-left point. 
       * @param x: x-value for top-left point.
       * @param y: y-value for top-left point. */
      void pos(float x, float y) { 
         pos(V2d<float> {x, y}); 
      };

      /** Setter for width of rectangle.
       * @param w: Width of rectangle. */
      void width(int w)  { m_w = w; };

      /** Setter for width of rectangle.
       * @param w: Width of rectangle. */
      void height(int h) { m_h = h; };

      /** Setter for angle of rectagle.
       * @param a: Angle of rectangle */
      void angle(float a) {
         V2d<float> unitV { 1.0f, 0.0f };
         unitV.rotate(a);
         set_point(1, pos() + unitV);
      }

      /** Getter for angle of rectangle.
       * @returns Angle of rectangle */
      float angle() const 
      {
         return (get_point(1) - get_point(0)).angle();
      }
      
   protected:
      float m_w = 0.0f;
      float m_h = 0.0f;

      void draw_stroke(Frame & frame);
      void draw_fill(Frame & frame);

      RectCorners get_corners() const;
   };
};
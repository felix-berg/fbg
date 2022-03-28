#ifndef RECT_HPP
#define RECT_HPP

#include "shape.hpp"
#include "../V2d.hpp"
#include "../frame.hpp"
namespace fbg {

   /** Rectangle class. 
    * @param pos(): Getting/setting point at top-left of rectangle.
    * @param width(): Getting/setting width of rectangle.
    * @param height(): Getting/setting height of rectangle.
   */
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
      void pos(V2d<float> p) { set_point(0, p); };

      /** Setter for top-left point. 
       * @param x: x-value for top-left point.
       * @param y: y-value for top-left point. */
      void pos(float x, float y) { set_point(0, V2d<float> {x, y}); };

      /** Setter for width of rectangle.
       * @param w: Width of rectangle. */
      void width(int w)  { m_w = w; };

      /** Setter for width of rectangle.
       * @param w: Width of rectangle. */
      void height(int h) { m_h = h; };

      /** Rotate shape around reference point. 
       * @param a: The angle to rotate the shape by.
       * @param ref: The centre of rotation. */
      void rotate(float a, const V2d<float> & ref);

      /** Rotate shape around x, y. 
       * @param a: Angle of rotation
       * @param x: x-value of reference point.
       * @param y: y-value of reference point. */
      void rotate(float a, float x, float y) { 
         Rect::rotate(a, {x, y});
      };

      /** Rotate shape around its first point (e.g. the center of a circle).
       * Max rotation is at about 0.0004f.
       * @param a: Angle to rotate by */
      void rotate(float a) { 
         m_angle += a;
      };

      void angle(float a) {
         m_angle = a;
      }

      float angle() const { return m_angle; };

   protected:
      void draw_stroke(Frame & f);
      void draw_fill(Frame & f);

   private:
      float m_w, m_h;
      float m_angle = 0.0f;
   };
};

#endif
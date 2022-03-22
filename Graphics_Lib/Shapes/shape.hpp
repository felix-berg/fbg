#ifndef SHAPE_HPP
#define SHAPE_HPP

#include <vector>
#include "../V2d.hpp"
#include "../rgba.hpp"
#include "../frame.hpp"

#include <cmath>

unsigned int get_unique_id();

namespace fbg {
   /** Shape class. Superclass for every shape.
    * Can be drawn by window object using Window::attach.
    * Cannot exist on its own. Instead use one of the existing subclasses. */
   class Shape {
   public:
      // make sure, a standalone "Shape" Object is not possible -> no constructor.
      Shape(const Shape &) = delete;
      Shape & operator = (const Shape &) = delete; 

      /** Getter for fill color.
       * @returns Current fill color of shape. */
      const Rgba & fill() const { return m_fill; };

      /** Getter for stroke color.
       * @returns Current stroke color of shape. */
      const Rgba & stroke() const { return m_stroke; };

      /** Getter for strokeweight
       * @returns Current strokeweight of shape. */
      int strokeweight() const { return m_strokeWeight; };
      
      /** Setter for fill color. 
       * @param c: Color */
      void fill(const Rgba & c) { m_fill = c; m_doFill = true; };

      /** Setter for fill color.
       * @param b: Grayscale value. [0, 255] */
      void fill(u_char b) { m_fill = {b, b, b, 255}; m_doFill = true; };

      /** Setter for fill color. 
       * @param b: Grayscale value. [0, 255]
       * @param a: Alpha value. [0, 255] */
      void fill(u_char b, u_char a) { m_fill = {b, b, b, a}; m_doFill = true; };

      /** Setter for fill color.
       * @param r: Amount of red. [0, 255] 
       * @param g: Amount of green. [0, 255]
       * @param b: Amount of blue. [0, 255]*/
      void fill(u_char r, u_char g, u_char b) { m_fill = {r, g, b, 255}; m_doFill = true; };

      /** Setter for fill color.
       * @param r: Amount of red. [0, 255] 
       * @param g: Amount of green. [0, 255]
       * @param b: Amount of blue. [0, 255] 
       * @param a: Amount of alpha. [0, 255] */
      void fill(u_char r, u_char g, u_char b, u_char a) { m_fill = {r, g, b, a}; m_doFill = true; }

      /** Setter for stroke color. 
       * @param c: Color */
      void stroke(const Rgba & c) { m_stroke = c; m_doStroke = true; };

      /** Setter for stroke color.
       * @param b: Grayscale value. [0, 255] */
      void stroke(u_char b) { m_stroke = {b, b, b, 255}; m_doStroke = true; };

      /** Setter for stroke color. 
       * @param b: Grayscale value. [0, 255]
       * @param a: Alpha value. [0, 255] */
      void stroke(u_char b, u_char a) { m_stroke = {b, b, b, a}; m_doStroke = true; };

      /** Setter for stroke color.
       * @param r: Amount of red. [0, 255] 
       * @param g: Amount of green. [0, 255]
       * @param b: Amount of blue. [0, 255]*/
      void stroke(u_char r, u_char g, u_char b) 	{ m_stroke = {r, g, b, 255}; m_doStroke = true; };

      /** Setter for stroke color.
       * @param r: Amount of red. [0, 255] 
       * @param g: Amount of green. [0, 255]
       * @param b: Amount of blue. [0, 255]
       * @param a: Amount of alpha. [0, 255] */
      void stroke(u_char r, u_char g, u_char b, u_char a) { m_stroke = {r, g, b, a}; m_doStroke = true; }

      /** Turn off rendering of stroke. */
      void noStroke() { m_doStroke = false; };
      /** Turn off rendering of fill. */
      void noFill()	 { m_doFill   = false; };

      /** Setter for stroke weight of shape.
       * @param sw: Stroke weight of shape. */ 
      void strokeweight(int sw)   { m_strokeWeight = sw; };

      /** Rotate shape around x, y.
       * @param a: Angle of rotation
       * @param x: x-value of reference point.
       * @param y: y-value of reference point. */
      void rotate(float a, float x, float y) { 
         V2d<float> ref {x, y};
         for (V2d<float> & p : m_points) {
            float c = (p - ref).size();
            p.x += cos(a) * c;
            p.y += sin(a) * c;
         }
      };

      /** Rotate shape 
       * @param a: Angle to rotate by */
      void rotate(float a) { 
         V2d<float> ref { get_point(0) };
         for (V2d<float> & p : m_points) {
            float c = (p - ref).size();
            p.x += cos(a) * c;
            p.y += sin(a) * c;
         }
      };

      /** Move shape 
       * @param v: Vector to move the shape by. */
      void move(const V2d<float> & p) { 
         for (V2d<float> & m_point : m_points)
            m_point += p;
      }

      /** Move shape 
       * @param x: x-value to move the shape by.
       * @param y: y-value to move the shape by. */
      void move(float x, float y) {
         for (V2d<float> & m_point : m_points) {
            m_point.x += x;
            m_point.y += y;
         }
      }

      /** Boolean equality of this shape to another. Uses the designated id of the shape.
       * @param other: The shape, that we are comparing against.
       * @returns True / false */
      bool operator == (const Shape & other) { return this->id() == other.id(); };

   protected:
      // Default constructor
      Shape() { 
         this->m_id = get_unique_id();
      };
      
      // Constructor with provided list of points
      Shape(std::initializer_list<V2d<float>> l) { 
         this->m_id = get_unique_id();

         for (auto & p : l)
            add_point(p);
      };

      // virtual function: to be defined by subclasses
      virtual void draw_stroke(Frame & f) = 0;
      virtual void draw_fill(Frame & f)   = 0;

      friend class Context; // allow for context to use draw_stroke and draw_fill
      
      // add given point to shape
      void add_point(const V2d<float> & p) 			{ m_points.push_back(p); };
      void set_point(int i, const V2d<float> & p) 	{ m_points[i] = p; };	
      
      // get point with given index
      const V2d<float> & get_point(int i) const { return m_points[i]; };

      // get number of points currently added to shape
      int num_points() const { return m_points.size(); };

      // get unique id
      unsigned int id() const { return m_id; };
      // generate a new id
      void generate_id() { m_id = clock(); };
      
      bool m_doStroke = true,
           m_doFill	 = true;

   private:
      std::vector<V2d<float>> m_points; // storage for the points of this shape
      unsigned int m_id; // unique id for identification

      Rgba m_fill = {255, 255, 255, 255};   // fill-in color
      Rgba m_stroke = {0, 0, 0, 255}; // color of stroke

      int m_strokeWeight = 1; // pixel width of stroke
   };
};
#endif
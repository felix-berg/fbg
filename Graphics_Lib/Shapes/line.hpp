#ifndef LINE_HPP
#define LINE_HPP

#include "shape.hpp"
namespace fbg {

   /** Line class. 
    * @param from(): Getting/setting of from-point.
    * @param to(): Getting/setting of to-point.
   */
   class Line : public Shape {
   public:
      static bool SMOOTH_EDGES;

      /** Constructor for line class.
       * @param f: The "from" point of the line.
       * @param t: The "to" point of the line.
      */
      Line(const V2d<float> & f, const V2d<float> & t)
         : Shape { { f, t } } {
         strokeweight(3);
         stroke(255);
      };

      /** Constructor for line class. 
       * @param fx: x-value of "from" point
       * @param fy: y-value of "from" point
       * @param tx: x-value of "to" point
       * @param ty: y-value of "to" point
      */
      Line(float fx, float fy, float tx, float ty)
         : Line { { fx, fy }, { tx, ty }} { };

      /** Default constructor for line class.
       * Sets "from" and to points to (0, 0). */
      Line() : Line {{0.0f, 0.0f}, {0.0f, 0.0f}} { };
      
      /** Getter for "from" point.
       * @returns "from" point. */
      V2d<float> from() const { return get_point(0); };
      /** Getter for "to" point.
       * @returns "to" point. */
      V2d<float> to()   const { return get_point(1); };
      
      /** Setter for "from" point.
       * @param f: Position of "from". */
      void from(const V2d<float> & f) { set_point(0, f); 			};
      /** Setter for "from" point.
       * @param x: x-value for "from" point. 
       * @param y: y-value for "from" point. */
      void from(float x, float y) 	  { set_point(0, {x, y});	};
      /** Setter for "to" point.
       * @param t: Position of "to". */
      void to(const V2d<float> & t)   { set_point(1, t); };
      /** Setter for "to" point.
       * @param x: x-value for "to" point. 
       * @param y: y-value for "to" point. */
      void to(float x, float y) 	 	  { set_point(1, {x, y});	};

   protected:
      void draw_stroke(Frame & f);
      void draw_fill(Frame & f)   { };
   };
};
#endif
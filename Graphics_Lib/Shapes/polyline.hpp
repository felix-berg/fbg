#ifndef POLYGON_HPP
#define POLYGON_HPP

#include "shape.hpp"

// TODO: Javadocs

namespace fbg {
   /** Polyline class. 
    * @param vertex(): Setter for a new vertex
    * @param set_vertex(): Setter for existing vertex
    * @param get_vertex(): Getter for existing vertex
    * @param open(): Open the polyline
    * @param close(): Close the polyline */
   class Polyline : public Shape {
   public:
      /** Constructor Polyline class. 
       * @param list: List of points for the polyline. */
      Polyline(std::initializer_list<V2d<float>> list) : Shape { list } { 
         // set default value of stroke
         stroke(0);
         strokeweight(1);
      };

      /** Default constructor for polyline class. */
      Polyline() : Shape { } {
         stroke(0);
         strokeweight(1);
      }

      /** Setter for new vertex. Adds the given point to the list of vertices in the polyline.
       * @param v: Vertex to add. */
      void vertex(const V2d<float> & v) { Shape::add_point(v); };
      
      /** Setter for new vertex. Adds the given point to the list of vertices in the polyline.
       * @param x: x-value for vertex to add.
       * @param y: y-value for vertex to add. */
      void vertex(float x, float y) { Shape::add_point({x, y}); };

      /** Setter for existing vertex. 
       * @param i: The index of the vertex to modify.
       * @param v: The value to give the vertex. */
      void set_vertex(int i, const V2d<float> & v) { Shape::set_point(i, v); };

      /** Getter for existing vertex.
       * @param i: The index of the vertex to return.
       * @returns Vertex with given index i. */
      V2d<float> get_vertex(int i) const { return Shape::get_point(i); };

      /** Open the polyline, so the first and last points are not connected. */
      void open()  { m_open = true; };

      /** Close the polyline, so the first and last points are connected. */
      void close() { m_open = false; };

   protected:
      void draw_stroke(Frame & f);
      void draw_fill(Frame & f);

   private:
      bool m_open = true;
   };
};

#endif
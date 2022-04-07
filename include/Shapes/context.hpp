#ifndef CONTEXT_HPP
#define CONTEXT_HPP

#include "shape.hpp"

#include <vector>
namespace fbg {
   /** A context that holds an array of Shapes and its subclasses.
    * @param attach(): Attaching of shapes to context.
    * @param detach(): Detaching of shapes to context. */
   class Context : public Shape {
   public:
      /** Constructor for Context class.
       * @param o: Origin point. */
      Context(const V2d<float> & o) : Shape { { o } } {
         constexpr V2d<float> unitV { 1.0f, 0.0f };
         add_point(o + unitV);
      };

      /** Constructor for Context class.
       * @param x: x-value for origin point
       * @param y: y-value for origin point */
      Context(float x, float y) : Context { { x, y }} { };

      /** Default constructor for Context class. 
       * Sets origin to (0, 0) */
      Context() : Context { { 0.0f, 0.0f } } {  };

      /** @return Number of shapes in the object */
      int numShapes() const { return m_shapes.size(); };

      /** @return Vector of pointers to the current shapes */
      const std::vector<Shape *> & getShapes() const { return m_shapes; };


      /** Attach any number of shapes to this context. 
       * @param ss: List of shapes to be added. */
      template <typename ... Shapes>
         requires ((... && std::convertible_to<Shapes *, Shape *>))
      void attach(Shapes & ... ss)
      {
         bool isSelf = false;
         ((isSelf = (isSelf || *((Shape *) this) == ss)), ...);

         if (isSelf)
            throw std::runtime_error("Context attach error: Cannot attach context to itself.\n");

         int invalidId = -1;
         ((invalidId = (!ss.isDrawable() ? ss.id() : invalidId)), ...);

         if (invalidId != -1)
            throw std::runtime_error(std::string("Added shape is invalid. Id: ") + std::to_string(invalidId));
         
         ((m_shapes.push_back(&ss)), ...);
      }

      /** Detach a shape from this context.
       * @param s: The shape to be removed */
      void detach (Shape & s) 
      { 
         for (int i = 0; i < m_shapes.size(); i++) 
            if (*m_shapes[i] == s) {
               detach(i);
            }
      };

      /** Set origin of the context. 
       * @param o: Origin of the context.  */
      void origin(const V2d<float> & o) 
      { 
         V2d<float> unitV { 1.0f, 0.0f };
         unitV.rotate(angle());
         set_point(0, o); 
         set_point(1, o + unitV);
      };

      /** Set origin of the context.
       * @param x: x-value for origin
       * @param y: y-value for origin */
      void origin(float x, float y) { Context::origin({x, y}); };

      /** Get origin of the context.
       * @returns Origin point for context. */
      const V2d<float> & origin() const { return get_point(0); };

      /** Setter for angle of context. 
       * @param a: Angle of context */
      void angle(float a) 
      { 
         V2d<float> unitV { 1.0f, 0.0f };
         unitV.rotate(a);
         set_point(1, origin() + unitV);
      };

      /** Getter for angle of context 
       * @returns Angle of context */
      float angle() const { return (get_point(1) - get_point(0)).angle(); };

      // copy undefined, because shapes are
      // stored as pointers.
      Context (const Context &) = delete;
      Context & operator = (const Context &) = delete;

   private:
      friend class Window; // allow for window to use draw_fill

      /** Empty: 
       * Drawing of shapes in ths context is done within the the draw_fill method. */
      void draw_stroke(Frame & f) {	};

      /** Draw pixels of every object in this context.
       * @param f: The Frame to draw the pixels to. */
      void draw_fill(Frame & f) 
      {
         float ang = angle();

         for (Shape * s : m_shapes) {
            s->rotate(ang, V2d<float> { 0.0f, 0.0f });
            s->move(origin());

            s->draw_fill(f);
            s->draw_stroke(f);

            s->move(V2d<float> {0, 0} - origin());
            s->rotate(-ang, V2d<float> { 0.0f, 0.0f });
         }
      }

      // private function
      void detach(int i) 
      {
         std::vector<Shape *>::iterator sit = m_shapes.begin() + i;  
         Shape * s = *sit;

         m_shapes.erase(m_shapes.begin() + i);
      }

      std::vector<Shape *> m_shapes;
   };
};
#endif
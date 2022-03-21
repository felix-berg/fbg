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
		Context() { };

		/** @return Number of shapes in the object */
		int numShapes() const { return m_shapes.size(); };
		/** @return Vector of pointers to the current shapes */
		const std::vector<Shape *> & getShapes() const { return m_shapes; };

		/** Attach a shape to this context.
		 * @param s: The shape to be added */
		void attach (Shape & s) { m_shapes.push_back(&s); };
		/** Detach a shape from this context.
		 * @param s: The shape to be removed */
		void detach (Shape & s) { 
			for (int i = 0; i < m_shapes.size(); i++) 
				if (*m_shapes[i] == s) 
					m_shapes.erase(m_shapes.begin() + i);
		};

	private:
		friend class Window; // allow for window to use draw_fill

		/** Empty: 
		 * Drawing of shapes in ths context is done within the the draw_fill method.
		 */
		void draw_stroke(Frame & f) {	};

		/** Draw pixels of every object in this context.
		 * @param f: The Frame to draw the pixels to. */
		void draw_fill(Frame & f) {
			for (Shape * s : m_shapes) {
				s->draw_fill(f);
				s->draw_stroke(f);
			}
		}

		std::vector<Shape *> m_shapes;
	};
};
#endif
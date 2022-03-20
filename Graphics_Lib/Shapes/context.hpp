#ifndef CONTEXT_HPP
#define CONTEXT_HPP

#include "shape.hpp"

#include <vector>
namespace fbg {
	class Context : public Shape {
	public:
		Context() { };
		void draw_stroke(Frame & f) {	};

		void draw_fill(Frame & f) {
			for (Shape * s : m_shapes) {
				s->draw_fill(f);
				s->draw_stroke(f);
			}
		}

		int num_shapes() const { return m_shapes.size(); };
		const std::vector<Shape *> & get_shapes() const { return m_shapes; };

		void attach (Shape & s) { m_shapes.push_back(&s); };
		void detach (Shape & s) { 
			for (int i = 0; i < m_shapes.size(); i++) 
				if (*m_shapes[i] == s) 
					m_shapes.erase(m_shapes.begin() + i);
		};

	private:
		std::vector<Shape *> m_shapes;
	};
};
#endif
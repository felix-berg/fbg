#ifndef LINE_HPP
#define LINE_HPP

#include "shape.hpp"
namespace fbg {
class Line : public Shape {
public:
	Line(const V2d<float> & f, const V2d<float> & t)
		: Shape { { f, t } } { };

	Line() : Line {{0.0f, 0.0f}, {0.0f, 0.0f}} { };
	
	Line(float fx, float fy, float tx, float ty)
		: Line { { fx, fy }, { tx, ty }} { };

	void compute_stroke(Frame & f);
	void compute_fill(Frame & f) { };

	// getters
	V2d<float> from() const { return get_point(0); };
	V2d<float> to()   const { return get_point(1); };
	
	// setters
	void from(V2d<float> f) 		 { set_point(0, f); 			};
	void from(float x, float y) 	 { set_point(0, {x, y});	};
	void to(const V2d<float> & p) { set_point(1, p); 			};
	void to(float x, float y) 		 { set_point(1, {x, y});	};
};
};
#endif
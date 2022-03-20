#ifndef POINT_HPP
#define POINT_HPP

#include "shape.hpp"
#include "drawing_algorithms/pixel.hpp"
namespace fbg {
class Point : public Shape {
public:
	Point(const V2d<float> & p) :
		Shape { p } { };
	
	Point() : Shape { V2d<float> {0.0f, 0.0f} } { };

	void draw_stroke(Frame & f) {
		if (!m_doStroke) return;
		V2d<int> p = pos();
		set_pixel(f, p.x, p.y, stroke(), strokeweight());
	}

	void draw_fill(Rgba *, int, int) { return; };

	// get position
	V2d<float> pos() const { return get_point(0); };
	
	// set position
	void pos(V2d<float> p)   { set_point(0, p); 	  };
	void pos(float x, float y) { set_point(0, {x, y}); };
};
};

#endif
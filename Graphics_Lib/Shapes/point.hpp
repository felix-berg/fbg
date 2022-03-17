#ifndef POINT_HPP
#define POINT_HPP

#include "shape.hpp"
#include "../alphacomposite.hpp"

class Point : public Shape {
public:
	Point(const V2d<int> & p) :
		Shape { p } { };
	
	Point() : Shape { V2d<int> {0, 0} } { };

	void compute_stroke(Rgba * pxs, int w, int h) {
		alpha_composite1(&pxs[pos().x + pos().y * w], &stroke()); 
	}

	void compute_fill(Rgba *, int, int) { return; };

	// get position
	V2d<int> pos() const { return get_point(0); };
	
	// set position
	void pos(V2d<int> p)   { set_point(0, p); 	  };
	void pos(int x, int y) { set_point(0, {x, y}); };
};

#endif
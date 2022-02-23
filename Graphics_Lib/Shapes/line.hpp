#ifndef LINE_HPP
#define LINE_HPP

#include "shape.hpp"

class Line : public Shape {
public:
	Line(const V2d<int> & f, const V2d<int> & t);
	Line() : Line {{0, 0}, {0, 0}} { };
	void compute_lines(Rgba *, int w, int h);
	V2d<int> from() const { return get_point(0); };
	V2d<int> to()   const { return get_point(1); };
	
	void from(V2d<int> f) { set_point(0, f); };
	void to(const V2d<int> & p) { set_point(1, p); };
};

#endif
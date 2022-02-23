#ifndef LINE_HPP
#define LINE_HPP

#include "shape.hpp"

class Line : public Shape {
public:
	Line(const V2d<int> & f, const V2d<int> & t);
	Line() : Line {{0, 0}, {0, 0}} { };
	std::vector<V2d<int>> compute_lines();
	V2d<int> get_from() const { return get_point(0); };
	V2d<int> get_to()   const { return get_point(1); };

	void set_from(const V2d<int> & p) { set_point(0, p); };
	void set_to(const V2d<int> & p) { set_point(1, p); };
};

#endif
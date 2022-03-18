#ifndef LINE_HPP
#define LINE_HPP

#include "shape.hpp"

class Line : public Shape {
public:
	Line(const V2d<int> & f, const V2d<int> & t);
	Line() : Line {{0, 0}, {0, 0}} { };
	
	void compute_stroke(Frame & f);
	void compute_fill(Frame & f) { };

	// getters
	V2d<int> from() const { return get_point(0); };
	V2d<int> to()   const { return get_point(1); };
	
	// setters
	void from(V2d<int> f) 		 { set_point(0, f); 			};
	void from(int x, int y) 	 { set_point(0, {x, y});	};
	void to(const V2d<int> & p) { set_point(1, p); 			};
	void to(int x, int y) 		 { set_point(1, {x, y});	};
};

#endif
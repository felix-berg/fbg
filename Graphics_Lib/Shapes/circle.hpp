#ifndef CIRCLE_HPP
#define CIRCLE_HPP

#include "shape.hpp"
#include "../V2d.hpp"
#include "../frame.hpp"

class Circle : public Shape {
public:
	Circle(const V2d<int> & p, int r) 
	: m_radius { r }	{
		add_point(p);
	}

	Circle() 
		: Circle({0, 0}, 1) { };

	void compute_stroke(Frame & f);
	void compute_fill(Frame & f) { };

	// getters
	V2d<int> pos() const { return get_point(0); };
	int radius() 	const { return m_radius; };
	
	// setters
	void pos(V2d<int> p) 	{ set_point(0, p); };
	void pos(int x, int y) 	{ set_point(0, V2d<int> {x, y}); };
	void radius(int r) 		{ m_radius = r; };

private:
	int m_radius;
};

#endif
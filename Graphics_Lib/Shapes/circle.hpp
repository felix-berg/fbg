#ifndef CIRCLE_HPP
#define CIRCLE_HPP

#include "shape.hpp"
#include "../V2d.hpp"
#include "../frame.hpp"
namespace fbg {
class Circle : public Shape {
public:
	Circle(const V2d<float> & p, float r) 
	: m_radius { r }	{
		add_point(p);
	}

	Circle(float x, float y, float r) : Circle { {x, y}, r} { };
	Circle() 
		: Circle({0, 0}, 1) { };

	void compute_stroke(Frame & f);
	void compute_fill(Frame & f);

	// getters
	V2d<float> pos() 	  const { return get_point(0); };
	float radius() 	  const { return m_radius; };
	
	// setters
	void pos(V2d<float> p) 		{ set_point(0, p); };
	void pos(float x, float y) { set_point(0, V2d<float> {x, y}); };
	void radius(float r) 		{ m_radius = r; };

private:
	float m_radius;
};
};
#endif
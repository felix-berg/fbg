#ifndef RECT_HPP
#define RECT_HPP

#include "shape.hpp"
#include "../V2d.hpp"

class Rectangle : public Shape {
public:
	Rectangle(const V2d<int> & p, int w, int h) 
	: m_w { w }, m_h { h }	{
		add_point(p);
	}

	Rectangle() 
		: Rectangle({0, 0}, 1, 1) { };

	void compute_stroke(Rgba *, int w, int h);
	void compute_fill(Rgba *, int w, int h);

	// getters
	V2d<int> pos() const { return get_point(0); };
	int width()	 	const { return m_w; };
	int height() 	const { return m_h; };
	
	// // setters
	void pos(V2d<int> p) { set_point(0, p); };
	void pos(int x, int y) { set_point(0, V2d<int> {x, y}); };
	void width(int w) 	{ m_w = w; 			 };
	void height(int h)	{ m_h = h; 			 };

private:
	int m_w = 0, 
		 m_h = 0;
};

#endif
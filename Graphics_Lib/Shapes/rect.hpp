#ifndef RECT_HPP
#define RECT_HPP

#include "shape.hpp"
#include "../V2d.hpp"
#include "../frame.hpp"

class Rect : public Shape {
public:
	Rect(const V2d<int> & p, u_int w, u_int h) 
	: m_w { w }, m_h { h }	{
		add_point(p);
	}

	Rect(int x, int y, u_int w, u_int h) : Rect { {x, y}, w, h } { };
	Rect() : Rect { {0, 0}, 1, 1 } { };

	void compute_stroke(Frame & f);
	void compute_fill(Frame & f);

	// getters
	V2d<int> pos() 	const { return get_point(0); };
	u_int width()	 	const { return m_w; };
	u_int height() 	const { return m_h; };
	
	// setters
	void pos(V2d<int> p) { set_point(0, p); };
	void pos(int x, int y) { set_point(0, V2d<int> {x, y}); };
	void width(int w) 	{
		if (w <= 0) throw std::runtime_error("Rectangle cannot have negative or zero width.\n");
		m_w = static_cast<u_int> ( w ); 			 
	};
	void height(int h)	{ 
		if (h <= 0) throw std::runtime_error("Rectangle cannot have negative or zero height.\n");
		m_h = static_cast<u_int> ( h );
	};

private:
	u_int m_w = 0, 
		   m_h = 0;
};

#endif
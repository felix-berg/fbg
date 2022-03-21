#ifndef RECT_HPP
#define RECT_HPP

#include "shape.hpp"
#include "../V2d.hpp"
#include "../frame.hpp"
namespace fbg {

	/** Rectangle class. 
	 * @param pos(): Getting/setting point at top-left of rectangle.
	 * @param width(): Getting/setting width of rectangle.
	 * @param height(): Getting/setting height of rectangle.
	*/
	class Rect : public Shape {
	public:
		/** Constructor for rectangle. 
		 * @param p: Top-left point.
		 * @param w: Width of rectangle.
		 * @param h: Height of rectangle.
		*/
		Rect(const V2d<float> & p, u_int w, u_int h) 
		: m_w { w }, m_h { h }	{
			add_point(p);
		}

		/** Constructor for rectangle. 
		 * @param x: x-value for top-left point.
		 * @param y: y-value for top-left point.
		 * @param w: Width of rectangle.
		 * @param h: Height of rectangle.
		*/
		Rect(float x, float y, u_int w, u_int h) : Rect { {x, y}, w, h } { };

		/** Default constructor for rectangle.
		 * Sets top-left point to (0, 0), width and height to 1. */
		Rect() : Rect { {0, 0}, 1, 1 } { };

		/** Getter for top-left point. 
		 * @returns Top-left point. */
		V2d<float> pos() 	const { return get_point(0); };
		
		/** Getter for the width of the rectangle. 
		 * @returns Width of rectangle. */
		u_int width()	 	const { return m_w; };

		/** Getter for the height of the rectangle.
		 * @returns Height of rectangle */
		u_int height() 	const { return m_h; };
		
		/** Setter for top-left point.
		 * @param p: Position for top-left point. */
		void pos(V2d<float> p) { set_point(0, p); };

		/** Setter for top-left point. 
		 * @param x: x-value for top-left point.
		 * @param y: y-value for top-left point. */
		void pos(float x, float y) { set_point(0, V2d<float> {x, y}); };

		/** Setter for width of rectangle.
		 * @param w: Width of rectangle. */
		void width(int w) 	{
			if (w <= 0) throw std::runtime_error("Rectangle cannot have negative or zero width.\n");
			m_w = static_cast<u_int> ( w ); 			 
		};

		/** Setter for width of rectangle.
		 * @param w: Width of rectangle. */
		void height(int h)	{ 
			if (h <= 0) throw std::runtime_error("Rectangle cannot have negative or zero height.\n");
			m_h = static_cast<u_int> ( h );
		};

	protected:
		void draw_stroke(Frame & f);
		void draw_fill(Frame & f);

	private:
		u_int m_w = 0, 
				m_h = 0;
	};
};

#endif
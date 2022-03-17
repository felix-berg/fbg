#ifndef SHAPE_HPP
#define SHAPE_HPP

#include <vector>
#include "../V2d.hpp"
#include "../rgba.hpp"

unsigned int get_unique_id();

class Shape {
public:
	// make sure, a standalone "Shape" Object is not possible -> no constructor.
	Shape(const Shape &) = delete;
	Shape & operator = (const Shape &) = delete; 

	// virtual function: to be defined by subclasses
	virtual void compute_stroke(Rgba * pixels, int w, int h)  { };
	virtual void compute_fill(Rgba * pixels, int w, int h) { };

	V2d<int> get_point(int i) const { return m_points[i]; };

	// get fill color of shape
	const Rgba & fill() const { return m_fill; };
	// get stroke color of shape
	const Rgba & stroke() const { return m_stroke; };
	// get stroke weight
	int stroke_weight() const { return m_stroke_weight; };
	

	// set fill color of object
	void fill(const Rgba & f)   { m_fill = f; };
	void fill(u_char brightness)   { m_fill 	= {brightness, brightness, brightness, 255}; };
	// set stroke color of object
	void stroke(const Rgba & s) { m_stroke = s; };
	void stroke(u_char brightness) { m_stroke = {brightness, brightness, brightness, 255}; };
	// set stroke weight for object
	void strokeweight(int sw)   { m_stroke_weight = sw; };

	// get current rotation in radians
	float angle() const { return m_angle; };
	// get current rotation in degrees
	float angle_degrees() const { return (m_angle / 6.28318530717958f) * 360.0f; }

	// set rotation in radians
	void angle(float a) { m_angle = a; };
	// set rotation in degrees
	void angle_degrees(float a) { m_angle = (a / 360.0f) * 6.28318530717958f; };

	bool operator == (const Shape & other) { return this->id() == other.id(); };

protected:
	// Default constructor
	Shape() { 
		this->m_id = get_unique_id();
	};
	
	// Constructor with provided list of points
	Shape(std::initializer_list<V2d<int>> l) { 
		this->m_id = get_unique_id();

		for (auto & p : l)
			add_point(p);
	};
	
	// add given point to shape
	void add_point(const V2d<int> & p) 			{ m_points.push_back(p); };
	void set_point(int i, const V2d<int> & p) { m_points[i] = p; };	

	// get unique id
	unsigned int id() const { return m_id; };
	// generate a new id
	void generate_id() { m_id = clock(); };
	
private:
	std::vector<V2d<int>> m_points; // storage for the points of this shape
	unsigned int m_id; // unique id for identification

	Rgba m_fill = {255, 255, 255, 255};   // fill-in color
	Rgba m_stroke = {255, 255, 255, 255}; // color of stroke

	int m_stroke_weight = 1; // pixel width of stroke

	float m_angle = 0; // current rotation of shape
};

#endif
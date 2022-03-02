#ifndef SHAPE_HPP
#define SHAPE_HPP

#include <vector>
#include "../V2d.hpp"
#include "../rgba.hpp"

unsigned int get_unique_id();

class Shape {
public:
	Shape(const Shape &) = delete;
	Shape & operator = (const Shape &) = delete;

	virtual void compute_lines(Rgba * pixels, int w, int h)  { };
	virtual void compute_fillin(Rgba * pixels, int w, int h) { };

	V2d<int> get_point(int i) const { return m_points[i]; };
	
	const Rgba & fill() const { return m_fill; };
	const Rgba & stroke() const { return m_stroke; };
	int stroke_weight() const { return m_stroke_weight; };
	
	void fill(const Rgba & f) { m_fill = f; };
	void stroke(const Rgba & s) { m_stroke = s; };
	void strokeweight(int sw) { m_stroke_weight = sw; };

	unsigned int id() const { return m_id; };

	void add_point(const V2d<int> & p) { m_points.push_back(p); };
	bool operator == (const Shape & other) { return this->id() == other.id(); };

protected:
	Shape() { 
		this->m_id = get_unique_id();
	};
	
	Shape(std::initializer_list<V2d<int>> l) { 
		this->m_id = get_unique_id();

		for (auto & p : l)
			add_point(p);
	};

	void set_point(int i, const V2d<int> & p) { m_points[i] = p; }; // m_points[i] = p
	void generate_id() { m_id = clock(); };

private:
	std::vector<V2d<int>> m_points;
	unsigned int m_id; // unique id for identification

	Rgba m_fill = {255, 255, 255, 255};
	Rgba m_stroke = {255, 255, 255, 255};

	int m_stroke_weight = 1;
};

#endif
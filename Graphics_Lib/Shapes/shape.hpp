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

	virtual std::vector<V2d<int>> compute_lines() { return m_points; };
	virtual std::vector<V2d<int>> compute_fillin() { return { }; };

	V2d<int> get_point(int i) const { return m_points[i]; };
	const Rgba & get_fill() const { return m_fill; };
	const Rgba & get_stroke() const { return m_stroke; };
	unsigned int id() const { return m_id; };

	void add_point(const V2d<int> & p) { m_points.push_back(p); };
	bool operator == (const Shape & other) { return this->id() == other.id(); };

protected:
	Shape() { };
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
};

#endif
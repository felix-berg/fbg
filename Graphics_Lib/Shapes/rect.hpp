#ifndef RECT_HPP
#define RECT_HPP

#include "shape.hpp"

class Rect : public Shape {
public:
	std::vector<V2d<int>> get_points();

private:

};

#endif
#ifndef BRESENHAM_HPP
#define BRESENHAM_HPP

#include <functional>
namespace fbg {
	void bresenham_line(int fx, int fy, int tx, int ty, std::function<bool(int x, int y)> set);
	void bresenham_circle(int r, std::function<void(int x, int y)> set);
}

#endif
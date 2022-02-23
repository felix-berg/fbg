#ifndef V2D_HPP
#define V2D_HPP

#include <cmath>
#include <ostream>

template <typename T>
struct V2d {
	T x, y;

	V2d<T> operator + (const V2d<T> & oth)  const { return { x + oth.x, y + oth.y }; };
	V2d<T> operator - (const V2d<T> & oth)  const { return { x - oth.x, y - oth.y }; };
	V2d<T> operator * (const double factor) const { return { (T) x * factor, (T) y * factor };	};
	V2d<T> operator / (const double factor) const { return { (T) x / factor, (T) y / factor };	};

	double size() 	  const { return std::sqrt(x*x + y*y); };
	double size_sq() const { return x*x + y*y; };
};

template <typename T> 
double dot_prod(const V2d<T> & v1, const V2d<T> & v2) {
	return v1.x * v2.x + v1.y * v2.y;
}

template <typename T>
std::ostream & operator << (std::ostream & os, const V2d<T> & v) {
	return (os << '(' << v.x << ", " << v.y << ')');
}

template <typename T>
V2d<T> operator * (const double factor, const V2d<T> & v) {
	return v * factor;
}

#endif
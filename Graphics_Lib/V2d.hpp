#ifndef V2D_HPP
#define V2D_HPP

#include <cmath>
#include <ostream>

template <typename T>
struct V2d {
	T x, y;

	V2d<T> operator + (const V2d<T> & oth)  const { return { x + oth.x, y + oth.y }; };
	V2d<T> operator - (const V2d<T> & oth)  const { return { x - oth.x, y - oth.y }; };
	V2d<T> operator * (const double factor) const { return { (T) (x * factor), (T) (y * factor) };	};
	V2d<T> operator / (const double factor) const { return { (T) (x / factor), (T) (y / factor) };	};

	V2d<T> & operator += (const V2d<T> & oth) {
		this->x += oth.x;
		this->y += oth.y;
		return *this;
	};

	V2d<T> & operator -= (const V2d<T> & oth) {
		this->x -= oth.x;
		this->y -= oth.y;
		return *this;
	};

	V2d<T> & operator *= (const double factor) {
		this->x *= factor;
		this->y *= factor;
		return *this;
	};

	V2d<T> & operator /= (const double factor) {
		this->x /= factor;
		this->y /= factor;
		return *this;
	};

	double size() 	  const { return std::sqrt(x*x + y*y); };
	double size_sq() const { return x*x + y*y; };
	bool is_bound(const V2d<T> & lower, const V2d<T> & upper) const {
		return ! (x < lower.x || x > upper.x ||
					 y < lower.y || y > upper.y);
	};
};

/*
	Return to added vectors, resulting in a vector of type T.
*/
template <typename T, typename S>
V2d<T> operator + (const V2d<T> & v1, const V2d<S> & v2) {
	return {
		v1.x + static_cast<T> ( v2.x ),
		v1.y + static_cast<T> ( v2.y )
	};
}

template <typename T, typename S>
V2d<T> operator - (const V2d<T> & v1, const V2d<S> & v2) {
	return {
		v1.x - static_cast<T> ( v2.x ),
		v1.y - static_cast<T> ( v2.y )
	};
}

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
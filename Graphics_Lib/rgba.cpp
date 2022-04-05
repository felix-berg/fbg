#include "rgba.hpp"

using namespace fbg;

std::ostream & fbg::operator << (std::ostream & os, Rgba color)
{
   return os << "(r: " << (int) color.r << ", "
             <<  "g: " << (int) color.g << ", "
             <<  "b: " << (int) color.b << ", "
             <<  "a: " << (int) color.a << ')';
};
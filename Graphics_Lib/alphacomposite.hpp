#ifndef ALPHACOMPOSIT_HPP
#define ALPHACOMPOSIT_HPP

#include "rgba.hpp"

typedef unsigned char u_char;
namespace fbg {
   void alpha_composite8(Rgba * dst, Rgba * over);
   void alpha_composite1(Rgba * dst, const Rgba * over);
   void alpha_compositeN(Rgba * dst, Rgba * over, int n);
   void alpha_compositeNC(Rgba * dst, const Rgba * over, int n);
};
#endif
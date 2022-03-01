#ifndef ALPHACOMPOSIT_HPP
#define ALPHACOMPOSIT_HPP

typedef unsigned char u_char;

struct Rgba;

void alpha_composit_16(Rgba * to, const Rgba * from);
void alpha_composit_n(Rgba * to, const Rgba * from, int n);
void alpha_composit_nc(Rgba * to, const Rgba * from, int n);
void alpha_composit_1(Rgba * to, const Rgba * from);
constexpr short fcs(u_char c);

#endif
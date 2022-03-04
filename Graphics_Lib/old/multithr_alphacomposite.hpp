#ifndef MULTITHR_ALPHACOMPOSITE_HPP
#define MULTITHR_ALPHACOMPOSITE_HPP

struct Rgba;

void init_compositor_threadpool(int scr_size);
void multithr_alpha_compositeN(Rgba * dst, Rgba * over);
void multithr_alpha_compositeNC(Rgba * dst, Rgba * over);


#endif 
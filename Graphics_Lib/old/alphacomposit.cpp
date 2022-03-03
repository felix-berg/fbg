#include "alphacomposite.hpp"

#include "rgba.hpp"

#include <iostream>
#include <immintrin.h>
#include <mmintrin.h>

void rgba_arr_to_16_vectors(__m256i * _r, __m256i * _g, __m256i * _b, __m256i * _a, const Rgba * array);
void print_avx_16u(const __m256i v);
u_short movemask_epi16(const __m256i & v);
template <typename T> void print_bytes(const T v, int bits, int grpw);
template <typename T> void print_bits(const T v, int bits, int grpw);
constexpr short fcs(u_char c);

/*
	Alpha composit two colors. Add over to dst.
	Standard CPU instructions.
*/
void alpha_composit_1(Rgba * dst, const Rgba * over) {
	if (over->a == 255) {
		*dst = *over;
		return;
	}

	u_char rest = 255 - over->a;

	*dst = Rgba {
		(u_char) (over->r + (dst->r * rest) / 255),
		(u_char) (over->g + (dst->g * rest) / 255),
		(u_char) (over->b + (dst->b * rest) / 255),
		(u_char) (over->a + (dst->a * rest) / 255)
	};
}

/*
	Multiply two _u256i with 16 16-bit unsigned shorts.
	Store result in _a.
*/
__m256i _zero = _mm256_set1_epi16(0); // set to 0b000...
__m256i _one  = _mm256_set1_epi16(fcs(1U)); // set to 0b00...01
void mult_epu16(__m256i * a, __m256i * b) {
	__m256i _min = _mm256_min_epu16(*a, *b);
	__m256i _max = _mm256_max_epu16(*a, *b);

	__m256i _is_zero;

	// a = a * b
	while (true) {
		// _min--; // but prevent underflow (saturated subtraction)
		_min = _mm256_subs_epu16(_min, _one);
		
		// min == 0
		_is_zero = _mm256_cmpeq_epi16(_min, _zero);

		// if (min == 0) break; 
		// only breaks if all calculation are finished
		if (movemask_epi16(_is_zero) == 0xFFFF)
			break;

		// max += max & is_zero;
		_max = _mm256_add_epi16(_max, _mm256_andnot_si256(_is_zero, _max));
	}

	*a = _max;
}

/*
	Thanks to: https://stackoverflow.com/questions/35285324/how-to-divide-16-bit-integer-by-255-with-using-sse
*/
inline __m256i DivideI16By255(__m256i value)
{
    return _mm256_srli_epi16(_mm256_add_epi16(
        _mm256_add_epi16(value, _mm256_set1_epi16(1)), _mm256_srli_epi16(value, 8)), 8);
}

/*
	Equivalent to _mm256_movemask_epi8().
	Returns a mask e.g. 0bxxxxxxxxxxxxxxxx as a u_short (16 bits),
	containing the most significant bits of the individual
	16-bit u_short integers.
*/
u_short movemask_epi16(const __m256i & v) {
	// get the mask from the 8-bit version
	// this vil for shorts give 2 bits per what we need:
	// 0b10101010 -> 0b1100110011001100
	int intmask = _mm256_movemask_epi8(v);

	u_short res = 0U;
	// loop through every pair of bits e.g. 11
	// of the integer mask, and convert it to a single bit
	for (int i = 0; i < sizeof(u_short) * 8; i++) {
		res += ((intmask >> (2 * i)) & 1) << i;
	}
	return res;
}

/*
	Composite arrays of colors of length 16.
	Using the "OVER" alpha compositing operator to put "over" over "dst".
*/

void alpha_composit_16(Rgba * dst, const Rgba * over) {
	// c2 on top of c1
	__m256i _r1, _b1, _g1, _a1;
	__m256i _r2, _b2, _g2, _a2;

	// load r, g, b and a values from the dst and over into _rx, _bx...
	rgba_arr_to_16_vectors(&_r1, &_b1, &_g1, &_a1, dst);
	rgba_arr_to_16_vectors(&_r2, &_b2, &_g2, &_a2, over);

	// uc255 = |255|255|255|... (but actually 127 as a char)
	//			  |FF|FF|FF|...
	u_char uc255 = 0xFF;
	__m256i _255 = _mm256_set1_epi16(fcs(uc255));

	// over.a == 255 
	__m256i _a2is255_cmp = _mm256_cmpeq_epi8(_a2, _255);
	u_short _a2is255_mask = movemask_epi16(_a2is255_cmp);

	// if (over.a == 255) {
	//		dst = over;
	//  	return;
	// }
	if (_a2is255_mask == 0xFFFF)  {  
		// create integer mask (0bXXXX...) from
		// every 16'th bit in _a2is255_mask
		// if == 11111111... the color is solid					 		
		_r1 = _r2;
		_g1 = _g2;
		_b1 = _b2;
		_a1 = _a2;

		return;
	}
	
	// rest = 255 - over.a
	__m256i _rest = _mm256_sub_epi16(_255, _a2);

	// ************************************
	// dst.x = over.x + (dst.x * rest) / 255
	// ************************************

	// dst.x = dst.x * rest;
	mult_epu16(&_r1, &_rest);
	mult_epu16(&_g1, &_rest);
	mult_epu16(&_b1, &_rest);
	mult_epu16(&_a1, &_rest);

	// dst.x /= 255;
	_r1 = DivideI16By255(_r1);
	_g1 = DivideI16By255(_g1);
	_b1 = DivideI16By255(_b1);
	_a1 = DivideI16By255(_a1);

	// dst.x += over.x;
	_r1 = _mm256_add_epi16(_r1, _r2);
	_g1 = _mm256_add_epi16(_g1, _g2);
	_b1 = _mm256_add_epi16(_b1, _b2);
	_a1 = _mm256_add_epi16(_a1, _a2);
}


/*
	This function is the "alpha_composit_16"-function, 
	but for chunks of any size <16 and >0
*/

void allocate_and_composite_n(Rgba * dst, const Rgba * over, int n) {
	if (n >= 16) 
		throw std::runtime_error("Cannot composite chunk");

	Rgba dst_buffered[16];
	Rgba over_buffered[16];

	memcpy(dst_buffered, dst, sizeof(Rgba) * n);
	memcpy(over_buffered, over, sizeof(Rgba) * n);	

	alpha_composit_16(dst_buffered, over_buffered);

	memcpy(dst, dst_buffered, sizeof(Rgba) * n);
}


/*
	Alpha composit two arrays of colors with n size.
*/

void alpha_composit_n(Rgba * dst, const Rgba * over, int n) {
	int i;
	// run through array and composit in 16-long chunks
	for (i = 0; i < n - 16; i += 16) {
		alpha_composit_16(dst + i, over + i);
	}
	int left = i + 16 - n;
	if (left != 0)
		std::cout << left << '\n';

};

void alpha_composit_nc(Rgba * dst, const Rgba * overc, int n) {
	int i;
	Rgba * over16 = (Rgba *) malloc(sizeof(Rgba) * 16);
	std::fill(dst, dst + 16, *overc);
	for (i = 0; i < n - 16; i += 16) {
		alpha_composit_16(dst + i, over16);
	}
	int left = i + 16 - n;
	if (left != 0)
		std::cout << left << '\n';
}


template <typename T>
void print_bytes(const T v, int bits, int groupwidth) {
	u_char * p = (u_char *) &v;
	for (int i = 0; i < (bits / 8); i++) {
		printf("%.2x", p[i]);
		if (i % groupwidth == groupwidth - 1)
			printf(" ");
	}
	printf("\n");
}

template <typename T>
void print_bits(const T v, int bits, int groupwidth) {
	for (int i = 0; i < bits; i++) {
		printf("%d", (v >> i) & 1);
		if (i % groupwidth == groupwidth - 1)
			printf(" ");
	}
	printf("\n");
}

/*
	Force c into being a short, copying bits
*/	
constexpr short fcs(u_char c) {
	return 0x00FF & *((char *) &c);
}

/*
	Convert length-16-array of Rgba objects into 4 vectors
	for the r, g, b and a channels respectfully
*/
void rgba_arr_to_16_vectors(__m256i * _r, __m256i * _g, __m256i * _b, __m256i * _a, const Rgba * array) {
	*_r = _mm256_set_epi16(
			fcs(array[0].r), fcs(array[1].r), fcs(array[2].r), fcs(array[3].r),
			fcs(array[4].r), fcs(array[5].r), fcs(array[6].r), fcs(array[7].r),
			fcs(array[8].r), fcs(array[9].r), fcs(array[10].r), fcs(array[11].r), 
			fcs(array[12].r), fcs(array[13].r), fcs(array[14].r), fcs(array[15].r));
	*_g = _mm256_set_epi16(
			fcs(array[0].g), fcs(array[1].g), fcs(array[2].g), fcs(array[3].g),
			fcs(array[4].g), fcs(array[5].g), fcs(array[6].g), fcs(array[7].g),
			fcs(array[8].g), fcs(array[9].g), fcs(array[10].g), fcs(array[11].g), 
			fcs(array[12].g), fcs(array[13].g), fcs(array[14].g), fcs(array[15].g));
	*_b = _mm256_set_epi16(
			fcs(array[0].b), fcs(array[1].b), fcs(array[2].b), fcs(array[3].b),
			fcs(array[4].b), fcs(array[5].b), fcs(array[6].b), fcs(array[7].b),
			fcs(array[8].b), fcs(array[9].b), fcs(array[10].b), fcs(array[11].b), 
			fcs(array[12].b), fcs(array[13].b), fcs(array[14].b), fcs(array[15].b));
	*_a = _mm256_set_epi16(
			fcs(array[0].a), fcs(array[1].a), fcs(array[2].a), fcs(array[3].a),
			fcs(array[4].a), fcs(array[5].a), fcs(array[6].a), fcs(array[7].a),
			fcs(array[8].a), fcs(array[9].a), fcs(array[10].a), fcs(array[11].a), 
			fcs(array[12].a), fcs(array[13].a), fcs(array[14].a), fcs(array[15].a));
}

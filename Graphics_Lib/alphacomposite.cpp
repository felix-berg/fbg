#include "alphacomposite.hpp"
#include "bitsandbytes.hpp"
#include <immintrin.h>

using namespace fbg;

void _mm_mult_epu8_into_2epu16(const __m256i _v1, const __m256i _v2, __m256i * _lRes, __m256i * _hRes);
inline __m256i _mm256_epu16_divideby256(const __m256i _v);
inline __m128i _mm_convertepi16_epi8(const __m256i _v);
inline __m256i _mm256_combine_2_128i(const __m128i _v1, const __m128i _v2);

/*
   Alpha composit two colors. Add over to dst.
   Standard CPU instructions.
*/
void fbg::alpha_composite1(Rgba * dst, const Rgba * over) 
{
   // if the alpha over the over color, 
   // it is completely opaque, 
   // so just return it
   if (over->a == 255) {
      *dst = *over;
      return;
   }

   // c = o * oa + d * (1 - oa)
   // o = (o * OA + d * (255 - OA)) / 255
   u_char rest = 255 - over->a;
   *dst = Rgba {
      (u_char) ((over->r * over->a + dst->r * rest) / 255),
      (u_char) ((over->g * over->a + dst->g * rest) / 255),
      (u_char) ((over->b * over->a + dst->b * rest) / 255),
      (u_char) ((over->a * over->a + dst->a * rest) / 255)
   };
}

void fbg::alpha_composite8(Rgba * dst, Rgba * over) 
{
   const __m256i _255 = _mm256_set1_epi8(0xFF);

   // c = (over.x * over.a + dst.x * (255 - over.a)) / 255
   // rest = 255 - over.a;
   // c = (over.x * over.a + dst.x * rest) / 255

   // inputs
   __m256i _dst, _over;

   // copy from over -> _over
   _mm256_storeu_si256(&_over, _mm256_loadu_si256((const __m256i *) over));

   // get a vector _oa filled with only the over.a value

   const __m256i _selectAlpha = _mm256_set1_epi32(0x000000FF);
   // _oa = over.a & _selectAlpha
   // _oa = |R1|G1|... & |00|00|00|FF| ==> |00|00|00|A1|...|00|A2|...
   __m256i _oa = _mm256_and_si256(_over, _selectAlpha);

   // set every element of the vector _oa to over.a
   // _oa = _oa & _oa << 1 byte & _oa << 2 byte & _oa << 3 byte
   // |00|00|00|A1|00|... ===> |A1|A1|A1|A1|A2|...
   _oa =_mm256_or_si256(_oa,										// _oa &
                        _mm256_or_si256(
                           _mm256_slli_si256(_oa, 1),    // _oa << 1 * 8&
                           _mm256_or_si256(
                              _mm256_slli_si256(_oa, 2), // _oa << 2 * 8 &
                              _mm256_slli_si256(_oa, 3)  // _oa << 3 * 8
                           )));

   // if (over.a == 255) return over;
   if (_mm256_movemask_epi8(_mm256_cmpeq_epi8(_oa, _255)) == 0xFFFFFFFF) {
      memcpy(dst, over, sizeof(Rgba) * 8);
      return;
   }
   
   // copy from dst -> _dst
   // structure: |R1|G1|B1|A1|R2|B2|...|G8|A8|
   _mm256_storeu_si256(&_dst, _mm256_loadu_si256((const __m256i *) dst));


   // _rest = 255 - over.a
   __m256i _rest = _mm256_sub_epi8(_255, _oa);

   // over.x + (dst.x * _rest) / 255
   // ******************************

   __m256i _lnDst16, _hnDst16; // new dst low part and high part
   // new dst = dst.x * _rest
   _mm_mult_epu8_into_2epu16(_dst, _rest, &_lnDst16, &_hnDst16);

   __m256i _nOver, _lnOver16, _hnOver16; // new over low part and high part
   // new over = (over.x * over.a) / 255
   _mm_mult_epu8_into_2epu16(_over, _oa, &_lnOver16, &_hnOver16);

   // use new dst for result to save registers
   // new dst = over.x * over.a + dst.x * rest
   _lnDst16 = _mm256_add_epi16(_lnOver16, _lnDst16);
   _hnDst16 = _mm256_add_epi16(_hnOver16, _hnDst16);

   // new dst = (over.x * over.x + dst.x * rest) / 255 (actually 256 because its faster)
   _lnDst16 = _mm256_epu16_divideby256(_lnDst16);
   _hnDst16 = _mm256_epu16_divideby256(_hnDst16);
   
   // convert to 8-bit integers again
   // use "_rest" register for result of (over.x * over.a + dst.x * rest) / 255
   // _rest = |_hnDst16|_lnDst16|
   _rest = _mm256_combine_2_128i(_mm_convertepi16_epi8(_lnDst16), _mm_convertepi16_epi8(_hnDst16));

   _mm256_storeu_si256((__m256i *) dst, _rest);
}

/*
   Computes n alpha composites for "over" OVER dst.
   Calls alpha_composite8 with chunks of eight.
   Requires that the number of elements n is divisible by eight.
*/
void fbg::alpha_compositeN(Rgba * dst, Rgba * over, int n) 
{
   // loop through all chunks of eight in the array
   for (int i = 0; i < n - 8; i += 8) {
      alpha_composite8(dst + i, over + i);
   }
   
   int n_pixelsLeft = n % 8;
   for (int i = n - n_pixelsLeft; i < n; i++) {
      alpha_composite1(dst + i, over + i);
   }
}

void fbg::alpha_compositeNC(Rgba * dst, const Rgba * over, int n) 
{
   if (n <= 0)
      return;

   // create array: |over|over|over|... length 8
   Rgba over8[8];
   std::fill(over8, over8 + 8, *over);

   // loop through all chunks of eight in the array
   for (int i = 0; i < n - 8; i += 8) {
      alpha_composite8(dst + i, over8);
   }

   int n_pixelsLeft = n % 8;
   if (n_pixelsLeft == 0)
      alpha_composite8(dst + n - 8, over8);

   for (int i = n - n_pixelsLeft; i < n; i++) {
      alpha_composite1(dst + i, over);
   }
}

/*
   Multiply char from _v1 and _v1 into two 16-bit short vectors _lRes (low result) and _hRes (high result).
   Note: Multiplication of signed integers and unsigned integers is the same operation
   Returns 16-bit versions of the product of the given vectors. 
*/
void _mm_mult_epu8_into_2epu16(const __m256i _v1, const __m256i _v2, __m256i * _lRes, __m256i * _hRes) 
{
   // "half"-registers
   __m128i _v1_128, _v2_128;
   // expanded registers, same as above, but with shorts
   __m256i _v1ex_256, _v2ex_256;

   // lower half bits of v1 and v2
   _v1_128 = _mm256_extractf128_si256(_v1, 0);
   _v2_128 = _mm256_extractf128_si256(_v2, 0);

   // "expand" to 256-bit vector with 16 shorts
   // instruction: "convert from epu8 to epi16" e.g.: |FF|EE| ==> |00|FF|00|EE|
   _v1ex_256 = _mm256_cvtepu8_epi16(_v1_128);
   _v2ex_256 = _mm256_cvtepu8_epi16(_v2_128);

   // multiply two vectors and return result
   // (actually multiplies the two 16 bit integers
   //  into and intermediary 32-bit integer, 
   //  and then returns the two low order
   //  bytes of the 32-bit int)
   *_lRes = _mm256_mullo_epi16(_v1ex_256, _v2ex_256);

   // upper half bits -> same thing
   _v1_128 = _mm256_extractf128_si256(_v1, 1);
   _v2_128 = _mm256_extractf128_si256(_v2, 1);

   // "expand" to 256-bit vector with 16 shorts
   _v1ex_256 = _mm256_cvtepu8_epi16(_v1_128);
   _v2ex_256 = _mm256_cvtepu8_epi16(_v2_128);

   // multiply two vectors and return result
   *_hRes = _mm256_mullo_epi16(_v1ex_256, _v2ex_256);
}


inline __m128i _mm_convertepi16_epi8(const __m256i _v) 
{
   // lowest 128 bits
   //			 0    1        7    8       15
   // _v: |00xx|00xy|...|00yz|00yy|...|00zz|
      //        v v v v v v v v v v v v v
      // => _lo128 |00|yy|...|00|zz|
               //   0   1     14 15

   __m128i _lo128 = _mm256_castsi256_si128(_v);

   // highest 128 bits
   //					 0    1    2
   // _v: 		 |xxxx|yyyy|zzzz|
      // 			v    v    v
   // _hi128: |xx|xx|yy|yy|zz|zz|
   __m128i _hi128 = _mm256_extractf128_si256(_v, 1);

   // 	|abcd|efgh| (16-bit)
   // 	  v  v  v
   //   |ab|cd|ef|gh| (8-bit)
   return _mm_packus_epi16(_lo128, _hi128);
}


inline __m256i _mm256_combine_2_128i(const __m128i _low, const __m128i _hi) 
{
   __m256i _empty;
   _empty = _mm256_insertf128_si256(_empty, _low, 0);
   _empty = _mm256_insertf128_si256(_empty, _hi, 1);
   return _empty;
}

/*
   Divides every 16-bit unsigned integer in the given 256-bit vector by 256
*/
inline __m256i _mm256_epu16_divideby256(const __m256i _v) 
{
   static const __m128i _shift8 = _mm_set_epi16(0x0000, 0x0000, 0x0000, 0x0008,
      0x0000, 0x0000, 0x0000, 0x0008);
   // v / 255 =~= v / 256 == v >> 8	
   return _mm256_srl_epi16(_v, _shift8);
}
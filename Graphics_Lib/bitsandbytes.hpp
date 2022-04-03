#ifndef BITSANDBYTES_HPP
#define BITSANDBYTES_HPP

typedef unsigned char uint8_t;
namespace fbg {
   template <typename T, typename F>
      inline T * force_into(F * v) {
         return ((T *) v);
      }

   template <typename T>
   void print_bytes(const T v, int bits, int bytesPerLine) 
   {
      uint8_t * p = (uint8_t *) &v;

      int bytes = bits / 8;

      for (int i = bytes - 1; i >= 0; i--) {
         printf("%.2x", p[i]);
         if (i % bytesPerLine == 0)
            printf(" ");
      }
      printf("\n");
   }

   template <typename T>
   void print_bits(const T v, int bits, int bitsPerLine) 
   {
      uint8_t * bytes = (uint8_t *) &v;

      int n_bytes = bits / 8;

      for (int i = n_bytes - 1; i >= 0; i--) {
         // for every byte:
         for (int bit = 0; bit < 8; bit++) {
            bool b = (bool) ((0b10000000 >> bit) & bytes[i]);
            printf("%c", b ? '1' : '0');

            if (((n_bytes - 1 - i) * 8 + bit) % bitsPerLine == bitsPerLine - 1)
               printf(" ");
         }	
      }
      printf("\n");
   }
};
#endif
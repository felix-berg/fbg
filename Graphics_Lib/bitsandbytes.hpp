#ifndef BITSANDBYTES_HPP
#define BITSANDBYTES_HPP

typedef unsigned char u_char;

template <typename T, typename F>
	inline T * force_into(F * v) {
		return ((T *) v);
	}

template <typename T>
void print_bytes(const T v, int bits, int bytes_per_line) {
	u_char * p = (u_char *) &v;

	int bytes = bits / 8;

	for (int i = bytes - 1; i >= 0; i--) {
		printf("%.2x", p[i]);
		if (i % bytes_per_line == 0)
			printf(" ");
	}
	printf("\n");
}

template <typename T>
void print_bits(const T v, int bits, int bits_per_line) {
	u_char * bytes = (u_char *) &v;

	int num_bytes = bits / 8;

	for (int i = num_bytes - 1; i >= 0; i--) {
		// for every byte:
		for (int bit = 0; bit < 8; bit++) {
			bool b = (bool) ((0b10000000 >> bit) & bytes[i]);
			printf("%c", b ? '1' : '0');

			if (((num_bytes - 1 - i) * 8 + bit) % bits_per_line == bits_per_line - 1)
				printf(" ");
		}	
	}
	printf("\n");
}

#endif
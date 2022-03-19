#include "shape.hpp"

using namespace fbg;

unsigned int get_unique_id() { // TODO: probably bad
static unsigned int unique_id_generator = 0;
	return unique_id_generator++; // lol 
}


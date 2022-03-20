#include "shape.hpp"

using namespace fbg;

unsigned int get_unique_id() { // TODO: probably bad
static unsigned int uniqueIdGenerator = 0;
	return uniqueIdGenerator++; // lol 
}


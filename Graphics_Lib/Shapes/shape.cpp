#include "shape.hpp"

using namespace fbg;

unsigned int get_unique_id() {
   static unsigned int uniqueIdGenerator = 0;
   return uniqueIdGenerator++;
}


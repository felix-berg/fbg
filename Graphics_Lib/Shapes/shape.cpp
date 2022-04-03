#include "shape.hpp"

using namespace fbg;

unsigned int get_unique_id() {
   static unsigned int uniqueIdGenerator = 0;
   return uniqueIdGenerator++;
}

bool fbg::operator == (const Shape & s1, const Shape & s2)
{
   return s1.id() == s2.id();
}
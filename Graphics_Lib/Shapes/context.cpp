#include "context.hpp"

using namespace fbg;

// TODO: Doesn't work
void Context::rotate(float a, const V2d<float> & p) 
{
   for (Shape * s : m_shapes)
      s->rotate(a, p - origin());

   // compare the angle of a point on the rectangle before and after rotation
   V2d<float> verticalLine = V2d<float>{10.0f, 10.0f};
   V2d<float> rotatedPoint = origin() + verticalLine;
   rotatedPoint = rotatedPoint - p;
   rotatedPoint.rotate(a);
   rotatedPoint += p;

   V2d<float> diff = origin() - p;
   diff.rotate(a);
   origin(p + diff);

   rotatedPoint -= origin();

   m_angle += angle_between(rotatedPoint, verticalLine);
};
#ifndef MATHS_HPP
#define MATHS_HPP

#include <cmath>

namespace fbpm {
   /** Random number generator.
    * @param min: Minimum value of generated number.
    * @param max: Maximum value of generated number.
    * @returns Random number between min and max of the type T. */
   template <typename T>
   T random(T min, T max) {
      return (double(rand()) / double(RAND_MAX)) * double(max - min) + min;
   }

   /** Random number generator.
    * @param max: Maximum value of generated number.
    * @returns Random number between 0 and max of the type T. */
   template <typename T>
   T random(T max) {
      return (double(rand()) / double(RAND_MAX)) * double(max);
   }
}

#endif
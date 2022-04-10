#ifndef MATHS_HPP
#define MATHS_HPP

#include <cmath>
#include <ostream>


namespace fbg {
   struct DivisionByZero : public std::runtime_error {
      DivisionByZero() : std::runtime_error { "Division by zero" } { };
   };

   constexpr float quarterPi = 0.7853981633f;
   constexpr float halfPi    = 1.5707963267f;
   constexpr float pi        = 3.1415926535f;
   constexpr float twoPi     = 6.2831853071f;

   /** Random number generator.
    * @param min: Minimum value of generated number.
    * @param max: Maximum value of generated number.
    * @returns Random number between min and max of the type T. */
   template <typename T>
   T random(T min, T max) 
   {
      return (double(rand()) / double(RAND_MAX)) * double(max - min) + min;
   }

   /** Random number generator.
    * @param max: Maximum value of generated number.
    * @returns Random number between 0 and max of the type T. */
   template <typename T>
      T random(T max) 
      {
         return (double(rand()) / double(RAND_MAX)) * double(max);
      }


   // /** TODO: untested */
   // float map(float value, float min1, float max1, float min2, float max2) {
   //    return ((value - min1) / (max1 - min1)) * (max2 - min2) + min2;
   // }
};

#endif
#ifndef MATHS_HPP
#define MATHS_HPP

#include <cmath>


namespace fbg {
   constexpr float quarterPi = 0.7853981633f;
   constexpr float halfPi    = 1.5707963267f;
   constexpr float pi        = 3.1415926535f;
   constexpr float twoPi     = 6.2831853071f;

   // template <typename T> T random(T min, T max);
   // template <typename T> T random(T max);
   // template <typename T> T sin(const T & value);
   // template <typename T> T cos(const T & value);
   // template <typename T> T tan(const T & value);
   // template <typename T> T asin(const T & value);
   // template <typename T> T acos(const T & value);
   // template <typename T> T atan(const T & value);

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
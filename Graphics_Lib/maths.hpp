#ifndef MATHS_HPP
#define MATHS_HPP

#include <cmath>


namespace fbg {
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

// template <typename T> 
//    T sin(const T & value) { 
//       if (std::is_same<T, float>::value) { return sinf(value); };

//       return static_cast<T> (std::sin(value)); 
//    };

// template <typename T> 
//    T cos(const T & value) { 
//       if (std::is_same<T, float>::value) { return cosf(value); };
      
//       return static_cast<T> (std::cos(value)); 
//    };

// template <typename T> 
//    T tan(const T & value) { 
//       if (std::is_same<T, float>::value) { return tanf(value); };
      
//       return static_cast<T> (std::tan(value)); 
//    };

// template <typename T> 
//    T asin(const T & value) { 
//       if (std::is_same<T, float>::value) { return asinf(value); };

//       return static_cast<T> (std::asin(value)); 
//    };

// template <typename T> 
//    T acos(const T & value) { 
//       if (std::is_same<T, float>::value) { return acosf(value); };
      
//       return static_cast<T> (std::acos(value)); 
//    };

// template <typename T> 
//    T atan(const T & value) { 
//       if (std::is_same<T, float>::value) { return atanf(value); };
      
//       return static_cast<T> (std::atan(value)); 
//    };
};

#endif
#ifndef V2D_HPP
#define V2D_HPP

#include <cmath>
#include <ostream>

#include "maths.hpp"

namespace fbg {
   // definition of number as either an integral (integer)
   // or a floating point number
   template <typename T>
      concept Number = std::integral<T> || std::floating_point<T>;

   /** Base 2d vector for the fbg graphics library. 
    * @param x: x-value for vector
    * @param y: y-value for vector
    * @param size(): Getter/setter for the size of the vector
    * @param size_sq(): Optimized getter for the size of the vector, squared.
    * @param normalize(): Normalizes the vector. Equivalent to size(1).
    * @param limit(): Limits the size of the vector. */
   template<Number T>
   struct V2d  {
      T x, y;

      V2d<T> operator + (const V2d<T> & oth)  const { return { x + oth.x, y + oth.y }; };
      V2d<T> operator - (const V2d<T> & oth)  const { return { x - oth.x, y - oth.y }; };
      V2d<T> operator * (const double factor) const { return { (T) (x * factor), (T) (y * factor) };	};
      V2d<T> operator / (const double factor) const { return { (T) (x / factor), (T) (y / factor) };	};

      V2d<T> & operator += (const V2d<T> & oth) {
         this->x += oth.x;
         this->y += oth.y;
         return *this;
      };

      V2d<T> & operator -= (const V2d<T> & oth) {
         this->x -= oth.x;
         this->y -= oth.y;
         return *this;
      };

      V2d<T> & operator *= (const double factor) {
         this->x *= factor;
         this->y *= factor;
         return *this;
      };

      V2d<T> & operator /= (const double factor) {
         this->x /= factor;
         this->y /= factor;
         return *this;
      };
      
      template <typename S>
      V2d<T> & operator = (const V2d<S> & oth) { 
         this->x = static_cast<T> (oth.x);
         this->y = static_cast<T> (oth.y);

         return *this;
      }

      template <typename S>
      operator V2d<S>() const { 
         return { 
            static_cast<S>(this->x), 
            static_cast<S>(this->y)
         };
      };

      /** @returns Size of vector */
      double size() 	  const { return std::sqrt(x*x + y*y); };

      /** Optimized. @returns Size of vector, squared. */
      double size_sq() const { return x*x + y*y; };
      
      /** Returns true if the point defined by the vector is within the given upper/lower bounds inclusive. 
       * @returns true / false */
      bool is_bound(const V2d<T> & lower, const V2d<T> & upper) const {
         return ! (x < lower.x || x > upper.x ||
                   y < lower.y || y > upper.y);
      };

      /** @returns Normalized vector with same direction. */
      V2d<T> normal() const {
         return *this / size();
      }

      /** Normalizes vector to have size 1. */
      void normalize() {
         *this /= this->size();
      }

      /** Setter for size of vector. 
       * @param sz: Wanted size. */
      void size(T sz) { 
         this->normalize();
         *this *= sz;
      }

      double angle() const 
      {
              if (x < 0) return pi + std::atan(y / x);
         else if (y < 0) return twoPi + std::atan(y / x);
         else            return std::atan(y / x);
      }

      void angle(float a) 
      {
         float sz = size();
         if (sz == 0)
            throw std::runtime_error("Cannot find angle of vector with size 0.");
         
         *this = {
            std::cos(a) * sz,
            std::sin(a) * sz
         };
      }
      
      /** Limit the size of the vector. 
       * @param l: The limit for the size of the vector. */
      void limit(T l) 
      {
         if (size() > l)
            size(l);
      }

      /** Rotate the vector by the given angle 
       * @param a: The angle to rotate the vector by. */
      void rotate(float a) 
      {
         T sina = std::sin(a);
         T cosa = std::cos(a);

         V2d<T> new_pos {
            cosa * this->x - sina * this->y,
            sina * this->x + cosa * this->y
         };

         *this = new_pos;
      }
   };

   template <typename T, typename S>
   V2d<T> operator + (const V2d<T> & v1, const V2d<S> & v2) 
   {
      return {
         v1.x + static_cast<T> ( v2.x ),
         v1.y + static_cast<T> ( v2.y )
      };
   }

   template <typename T, typename S>
   V2d<T> operator - (const V2d<T> & v1, const V2d<S> & v2) 
   {
      return {
         v1.x - static_cast<T> ( v2.x ),
         v1.y - static_cast<T> ( v2.y )
      };
   }

   /** Dot-product operator for vectors. v1 · v2
    * @param v1: First vector
    * @param v2: Second vector
    * @returns Dot product between vector v1 and vector v2. */
   template <typename T> 
   double dot_prod(const V2d<T> & v1, const V2d<T> & v2) 
   {
      return v1.x * v2.x + v1.y * v2.y;
   }

   /** @param v1: First vector
    * @param v2: Second vector
    * @returns The angle between the given vectors. */
   template <typename T>
   double angle_between(const V2d<T> & v1, const V2d<T> & v2) 
   {
      return std::acos(dot_prod(v1, v2) / (v1.size() * v2.size()));
   }

   template <typename T>
   std::ostream & operator << (std::ostream & os, const V2d<T> & v) 
   {
      return (os << '(' << v.x << ", " << v.y << ')');
   }


   template <typename T>
   V2d<T> operator * (const double factor, const V2d<T> & v) 
   {
      return v * factor;
   }

   /** @returns a random vector from (0, 0) to (maxX, maxY). */
   template <typename T>
   V2d<T> random_vector(T maxX, T maxY) 
   {
      return { 
         fbg::random<T>(maxX), 
         fbg::random<T>(maxY) 
      };
   };

   /** @returns a random vector from (minX, minY) to (maxX, maxY). */
   template <typename T>
   V2d<T> random_vector(T minX, T maxX, T minY, T maxY) 
   {
      return {
         fbg::random(minX, maxX),
         fbg::random(minY, maxY)
      };
   }
};

#endif
#ifndef BOID_HPP
#define BOID_HPP

#include "2D_Graphics_Lib.hpp"

using namespace fbg;

struct Boid : public Rect {
   float maxSteerForce = 40.0f; // default values
   float maxSpeed      = 200.0f;

   V2d<float> velocity {0, 0};
   V2d<float> acceleration {0, 0};

   Boid(float x, float y) : Rect { x, y, 6.0f, 15.0f } 
   {
      fill(255);
      noStroke(); 
   };

   Boid() : Boid { 0, 0 } { };

   void update(float dt) {
      velocity += acceleration * dt;
      
      velocity.limit(maxSpeed);
      
      move(velocity * dt);

      this->angle(velocity.angle() - halfPi);

      acceleration = {0, 0};
   }
   
   void seek_velocity(const V2d<float> & desiredVel) {
      V2d<float> steerAcc = desiredVel - velocity;

      steerAcc.size(maxSteerForce);

      acceleration += steerAcc;
   }

   void seek_point(const V2d<float> & point) {
      V2d<float> desiredVel = point - pos();
      float d = desiredVel.size();

      seek_velocity(desiredVel);
   }
};

struct Flock {
   std::vector<Boid> boids;

   float alignDistance = 50.0f;
   LoopWin & win;


   Flock(LoopWin & w, int num) : win { w } {
      boids.resize(num);

      for (int i = 0; i < num; i++) {
         boids[i].pos(random_vector(win.width(), win.height()));
         boids[i].velocity = random_vector(-100.0f, 100.0f, -100.0f, 100.0f);
         win.attach(boids[i]);
      }
   }

   void wrap_boids_to_screen() {
      for (Boid & boid : boids) {
         V2d<float> p { boid.pos() };

         if (p.x < 0)             p.x += win.width();
         if (p.x >= win.width())  p.x -= win.width();
         if (p.y < 0)             p.y += win.height();
         if (p.y >= win.height()) p.y -= win.height();
         
         boid.pos(p);
      }
   }

   void align_boids() {
      for (Boid & boid : boids) {
         int numFound = 0;
         V2d<float> velocitySum {0, 0};

         for (Boid & other : boids) {
            if (boid == other) continue;

            float dist = (other.pos() - boid.pos()).size();
            if (dist < alignDistance) {
               velocitySum += other.velocity;
               numFound++;
            }
         }

         if (numFound == 0) {
            boid.fill(255, 100);
            continue;
         } else
            boid.fill(255, 255);


         boid.seek_velocity(velocitySum); // seek average velocity
      }
   }

   void update(float dt) {
      align_boids();

      for (Boid & b : boids) {
         b.update(dt);
      }

      wrap_boids_to_screen();
   }
   

   int numBoids() const { return boids.size(); };
};

#endif
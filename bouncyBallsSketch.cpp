#include "2D_Graphics_Lib.hpp"

using namespace fbg;

class Particle : public Circle {
public:
   V2d<float> vel;

   Particle(float x, float y, float r)
      : Circle { x, y, r },
        vel { fbg::random_vector(-1.0f, 1.0f, -1.0f, 1.0f) }
   {
      stroke(140);
      noFill();
      strokeweight(3);
   }

   void update(float dt)
   { 
      this->move(vel * dt);
   }

   bool collides_with(const Particle & other)
   {
      float thr = this->radius();
      float otr = other.radius();

      return ((other.pos() - pos()).size() < thr + otr);
   }

   void bounce_on_walls(const Window * w)
   {
      V2d<float> p { pos() };
      float r { radius() };

      if (p.x - r < 0 || p.x + r >= w->width())  vel.x *= -1;
      if (p.y - r < 0 || p.y + r >= w->height()) vel.y *= -1;
   }
};

class ParticleSystem : public Context {
public:
   ParticleSystem(size_t num, const Window * w)
      : Context { }, windowPtr { w }
   { 
      m_particles.reserve(num);

      float particleRadius = 20.0f;

      for (size_t i = 0; i < num; i++) {
         m_particles.push_back(Particle {
            (float) random(particleRadius, w->width()  - particleRadius),
            (float) random(particleRadius, w->height() - particleRadius),
            particleRadius
         });

         this->attach(m_particles.back());
      }
   }

   void update(float dt)
   {

      std::for_each(m_particles.begin(), m_particles.end(), [](auto & p) { p.stroke(140); });

      for (size_t i = 0; i < m_particles.size(); i++) {
         Particle & p = m_particles[i];
         for (size_t j = i + 1; j < m_particles.size(); j++) {
            Particle & o = m_particles[j];

            if (p.collides_with(o)) {
               p.vel += (p.pos() - o.pos());
               o.vel += (o.pos() - p.pos());
               
               p.stroke(255);
               o.stroke(255);
            }
         }
      }

      for (Particle & p : m_particles) {
         p.bounce_on_walls(windowPtr);
         p.update(dt);
      }
   }

private:
   std::vector<Particle> m_particles;
   const Window * windowPtr;
};

int main() 
{
   LoopWin win { "Bouncy Balls!", 640, 480 };

   ParticleSystem ps { 20, &win };

   win.attach(ps);

   win.draw = [&](float dt) {
      ps.update(dt);
   };
   
   win.run();

   return 0;
}
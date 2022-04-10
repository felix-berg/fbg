#include "fbg.hpp"

using namespace fbg;

class Particle : public Circle {
public:
   V2d<float> velocity;

   Particle(float x, float y, float r)
      : Circle { x, y, r },
        velocity { fbg::random_vector(-1.0f, 1.0f, -1.0f, 1.0f) }
   {
      srand(clock());
      noStroke();
      fill(rand() % 255,
           rand() % 255,
           rand() % 255,
           140);
      strokeweight(3);
   }

   void update(float dt)
   { 
      this->move(velocity * dt);
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

      if (p.x - r < 0 || p.x + r >= w->width())  velocity.x *= -1;
      if (p.y - r < 0 || p.y + r >= w->height()) velocity.y *= -1;
   }

   void set_alpha(uint8_t a)
   {
      Rgba old = fill();
      fill(old.r, old.g, old.b, a);
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

      std::for_each(m_particles.begin(), m_particles.end(), [](auto & p) { 
         p.set_alpha(140);
      });

      for (size_t i = 0; i < m_particles.size(); i++) {
         Particle & p = m_particles[i];
         for (size_t j = i + 1; j < m_particles.size(); j++) {
            Particle & o = m_particles[j];

            if (p.collides_with(o)) {
               p.velocity += (p.pos() - o.pos());
               o.velocity += (o.pos() - p.pos());

               p.set_alpha(255);
               o.set_alpha(255);
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

   win.framerate(140);

   ParticleSystem ps { 20, &win };

   win.attach(ps);

   win.draw = [&](float dt) {
      ps.update(dt);
   };
   
   win.run();

   return 0;
}
#include "2D_Graphics_Lib.hpp"

#include <vector>

using namespace fbg;

class Grid : public Context {
public:
   Grid(int cell_size, const LoopWin & window)
      : m_cell_size { cell_size }
   {
      if (window.width()  % m_cell_size != 0 ||
          window.height() % m_cell_size != 0) 
         throw std::runtime_error("Window width and height have to be divisible by cell size."); 

      m_gridw = window.width()  / m_cell_size;
      m_gridh = window.height() / m_cell_size;   

      m_lines.reserve(m_gridw * m_gridh);
      m_velocities.reserve(m_gridw * m_gridh);
      m_points.reserve(m_gridw * m_gridh);

      for (int gy = 0; gy < m_gridh; gy++) {
         for (int gx = 0; gx < m_gridw; gx++) {
            m_velocities.push_back(fbg::random_vector(-1.0f, 1.0f, -1.0f, 1.0f));
            m_velocities.back().normalize();

            const float csz = static_cast<float> (m_cell_size);

            V2d<float> scrP {
               gx * csz + csz / 2,
               gy * csz + csz / 2
            };

            V2d<float> direction = m_velocities.back();
            direction.size(static_cast<float> (m_cell_size) * 0.5f);
            
            m_lines.push_back(Line {
               scrP,
               scrP + direction
            });

            m_points.push_back(Point { scrP });
            m_points.back().stroke(0);
            m_points.back().strokeweight(5);
            
            this->attach(m_lines.back());
            this->attach(m_points.back());
         }
      }
   }

   V2d<float> velocity_at(const V2d<float> & p) const
   {
      int x = std::round(p.x / static_cast<float> (m_cell_size));
      int y = std::round(p.y / static_cast<float> (m_cell_size));

      if (x < 0 || x >= m_gridw ||
          y < 0 || y >= m_gridh) 
            return { 0, 0 };

      return m_velocities[y * m_gridw + x];
   }

private:
   std::vector<Line> m_lines;
   std::vector<Point> m_points;
   std::vector<V2d<float>> m_velocities;
   int m_gridw;
   int m_gridh;
   const int m_cell_size;
};

class Particle : public Circle {
public:
   V2d<float> vel {};
   V2d<float> acc {};

   Particle() : Particle { 0, 0 } { };

   Particle(float x, float y)
      : Circle { x, y, 8.0f }
   { }

   void update(float dt)
   {
      vel += acc * dt;
      this->move(vel * dt);
      acc = { 0, 0 };
   }
};

class ParticleSystem : public Context {
public:
   float gridPushingStrength = 50.0f;

   ParticleSystem(int num, const LoopWin & win) : Context { } 
   {
      m_particles.reserve(num);
      for (size_t i = 0; i < num; i++) {
         m_particles.push_back({
            fbg::random(win.width()), 
            fbg::random(win.height())});
         
         this->attach(m_particles.back());
      }
   }

   void update(const Grid & grid, float dt) 
   {
      for (Particle & p : m_particles) {
         p.acc += grid.velocity_at(p.pos()) * gridPushingStrength;
         p.update(dt);
      }
   }

   void wrap_to_screen(const LoopWin & win) {
      for (Particle & particle : m_particles) {
         V2d<float> p { particle.pos() };
         if (p.x < 0)            p.x += win.width();
         if (p.x >= win.width()) p.x -= win.width();

         if (p.y < 0)             p.y += win.height();
         if (p.y >= win.height()) p.y -= win.height();

         particle.pos(p);
      }
   }

private:
   std::vector<Particle> m_particles;
};

int main() {
   fbg::LoopWin win { "Vector grid test", 800, 800 }; 

   Grid grid { 40, win };
   ParticleSystem ps { 100, win };

   win.attach(grid);
   win.attach(ps);

   win.draw = [&](float dt) -> void {
      ps.wrap_to_screen(win);
      ps.update(grid, dt);
   };

   win.run();

   return 0;
}
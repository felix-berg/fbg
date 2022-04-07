# 2D Graphics Library for C++!



## Simple sketch
```C++
#include "fbg.hpp"

using namespace fbg;

int main() {
   // Construct 1280x720 Window object.
   LoopWin window { "Window Title", 1280, 720 };

   // set window attributes:
   window.background(0, 50); // set background color to black with alpha 50
   window.framerate(112.0f);
   
   // Create circle object at (0, 0) with radius 50
   Circle circle {0.0f, 0.0f, 50.0f};
   
   // draw circle object at every Frame update,
   // by attaching it to the window object
   window.attach(circle);

   // define function that will be called at every frame
   window.draw = [&](float frametime) {
      // grab the mouse position from the window
      V2d<int> mpos = window.mouse();
   
      // set the position of the radius to the
      // current mouse position
      circle.pos(mpos);
   };

   // Start the game loop by calling run.
   window.run(); 

   return 0; // :)
}
```

Produces output:
![Final output](https://user-images.githubusercontent.com/93908883/159099446-f5976859-6240-45c8-a9e5-a05dad4b5cb2.png)


## Particle-system: Bouncy-balls
The following example produces the following output:
![image](https://user-images.githubusercontent.com/93908883/161541847-820393f2-ca34-48e9-9ac0-7109823e78f2.png)

The example shows how the `fbg::Shape` derived classes can be expanded upon to simplify graphics programming.

### Particle (extends `Circle`)
The bouncy balls are all instances of the `Particle` class:
```C++

class Particle : public Circle {
public:
   V2d<float> velocity;

   Particle(float x, float y, float radius);

   void update(float dt);
   bool collides_with(const Particle &);
   void bounce_on_walls(const Window *);
};
```

The `Particle::update(float)`-method simply adds the current velocity to the position of the Particle, using the inherited `Circle::move`-method.

```C++
void Particle::update(float dt)
{
   move(velocity * dt);
}
```
The `collides_with` method simply checks whether the distance to the other particle is less than their combined radii:
```C++
bool Particle::collides_with(const Particle & other) 
{
   return (other.pos() - this->pos()).size() 
           < other.radius() + this->radius();
}
```
Here we again see the use of inherited methods to simplify the class definition.

The `bounce_on_walls`-method compares the position of the particle against the boundaries of the given window. In the function, the velocity is updated, if a collision i detected.

```C++
void Particle::bounce_on_walls(const Window * w)
{
   V2d<float> p = pos();
   float r = radius();

   // bounce on walls
   if (p.x - r < 0 || p.x + r >= w->width())  
      velocity.x *= -1;

   // bounce on ceiling
   if (p.y - r < 0 || p.y + r >= w->height()) 
      velocity.y *= -1;
}
```

Because the `Particle`-class is derived from a shape, the standard `Window::attach` or `Context::attach` methods can be used on it, just as if the particle was any other shape (e.g. `Circle` or `Rect`).

```C++
Context con { /* ... */ };
Particle p  { /* ... */ };
con.attach(p);
```

### ParticleSystem (extends `Context`)
This is used with another `Shape`-derived class, namely the `ParticleSystem`-class, which extends `Context`.

```C++
class ParticleSystem : public Context {
public:
   // construct and attach vector of particles
   Particle(size_t num, const Window * w);

   // update all particles (call .update(), etc...)
   void update(float dt);

private:
   // container for particles
   std::vector<Particle> m_particles;
};
```

This allows the `Particlesystem` to both act as a container of particles, and a context for every particle in the sketch.

In the `ParticleSystem`-constructor, the vector is filled with `Particle`-instances, which are attached to itself:
```C++
Particle::Particle(size_t num, const Window * w) 
   // ...
{
   for (size_t i = 0; i < num; i++) {
      m_particles.push_back(Particle { /* Random starting value */ });

      // Set particle parameters
      // ...

      // Attach new particle to this Particlesystem,
      // using inherited Context::attach method.
      Particle & newP = m_particles.back();
      this->attach(newP); 
   }
}
```

In the `ParticleSystem::update`-method, all particles are updated, and collisions between the particles are detected.
```C++
void ParticleSystem::update(float dt)
{
   // ...

   // check every particle p against every other
   // particle o, and react to potential collision
   for (size_t i = 0; i < m_particles.size(); i++) {
      Particle & p = m_particles[i];
      
      for (size_t j = i + 1; j < m_particles.size(); j++) {
         Particle & o = m_particles[j];

         if (p.collides_with(o)) {
            // collision detected

            // reaction
            p.velocity += (p.pos() - o.pos());
            o.velocity += (o.pos() - p.pos());

            // use of inherited method Shape::stroke
            p.stroke(255);
            o.stroke(255);
         }
      }
   }

   // updating of positions
   for (Particle & p : m_particles) {
      p.bounce_on_walls(windowPtr);
      p.update(dt);
   }
}
```

Finally, an instance of `ParticleSystem` is created, and it is attached to a window, again, since `ParticleSystem` is derived from `Context`.
```C++
#include "2D_Graphics_Lib"
using namespace fbg;
int main() 
{
   LoopWin win { /* ... */ };

   // initialize particlesystem with 40 particles
   ParticleSystem ps { 40, &win };

   // attach created particle system to window
   win.attach(ps);

   // update the particlesystem every frame
   win.draw = [&](float dt) {
      ps.update(dt);
   };

   // run animation
   win.run();

   return 0; // :)
}
```

The full source-code for the example can be found in `demos/bouncyBallsSketch.cpp`

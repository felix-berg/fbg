# 2D Graphics Library for C++!
This is a 2D graphics library, built from scratch using C++. The only dependency of the library is SDL, which is used as a pixelbuffer and for handling input.
This means, that the every pixel drawn to the screen, i have drawn myself. 
The library is inspired by [processing](https://processing.org) and [p5.js](https://p5js.org), but takes a more object-oriented approach to drawing objects to the screen. I.e. instead of a global `draw_line(x1, y1, x2, y2)`-function, you will instead create a `fbg::Line`-object and attach it to a window object using the `Window::attach(line)`-method.

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

## Main Classes 
#### `fbg::V2d<T>`: The 2D-vector
This class is the main vector class used by the library. An `fbg::V2d` vector has an `x` and a `y`-value. These values are of the specified type `T`:
```C++
template <typename T>
struct V2d { 
   T x, y; 
   /* ... */ 
};
```
This class has many available matematical operators and functions. Instead of explaining each one, here is the summary:
##### Methods
```C++
// Basic arithmetic (+, -, *, /)
V2d<T>   operator +  (const V2d<T> & oth) const;
V2d<T>   operator -  (const V2d<T> & oth) const;
V2d<T> & operator += (const V2d<T> & oth);
V2d<T> & operator -= (const V2d<T> & oth);

V2d<T>   operator *  (double d) const;
V2d<T>   operator /  (double d) const;
V2d<T> & operator *= (double d);
V2d<T> & operator /= (double d);

// Mathematical functions
double V2d<T>::size() const;
double V2d<T>::size_sq() const;  // size * size -> more efficient

void   V2d<T>::size(double sz);  // setter

V2d<T> V2d<T>::normal() const;
void   V2d<T>::normalize();
void   V2d<T>::limit(double sz); // limits size of vector

double V2d<T>::angle() const;    // returns angle againt x-axis (positive angle = clockwise)
void   V2d<T>::angle(float a);   // setter
void   V2d<T>::rotate(float a);

bool is_bound(const V2d<T> & lower,        // true if point at end of vector is within 
              const V2d<T> & upper) const; // bounding box defined by [lower; upper]  
```

#### `fbg::Window`: The static window. 
This class is used for creating static images, or user-controlled animations. When an instance of the `fbg::Window` class is created, a window is created an initialized. After this, objects derived from the `fbg::Shape`-class can be attached to the window using `Window::attach`.
```C++
int main() 
{
   Window window { <Title in top bar>, <width>, <height>};
   Line line { 50, 50, 100, 100 };
   window.attach(line);

   // ...
```

The attached shapes are drawn to then window, when the `Window::update` method is called. Don't forget to have a way to stop the program from exiting, though. Here, i just use `Window::wait_for_key`, with no arguments, which simply waits until any key is pressed.
```C++
   // ...  
   window.update();
   window.wait_for_key();

   return 0;
}
```

##### Methods:
```C++
void Window::background(Rgba color);   // Setter for background color.
void Window::background(uint8_t r, uint8_t g, uint8_t b, uint8_t a);
void Window::background(uint8_t r, uint8_t g, uint8_t b);
void Window::background(uint8_t brightness, uint8_t alpha);
void Window::background(uint8_t brightness);

void Window::attach(Shapes & ... ss);  // Attach any number of shapes to this Window.
void Window::detach(Shape & s)         // Detach shape from window.

void Window::update();                 // Render the currently attached shapes to the window

void Window::wait_for_key(Key k);      // Wait for given key to be pressed
void Window::wait_for_key();           // Wait for any key to be pressed

int Window::width();                   // sets/gets the current width of the window
int Window::height();                  // sets/gets the current height of the window

V2d<int> Window::dimensions();         // returns a vector with coordinates (width, height);
```


#### `fbg::LoopWin`: The dynamic window
This class is derived from `fbg::Window`, which means, that all methods  of  `fbg::Window` can be used. 
The difference between the two classes, is that the `fbg::LoopWin` object has the method `LoopWin::run`, which starts a loop at 60 fps. The attached objects to the window are drawn at every frame of this loop.
Furthermore, the loop calls the user-defined `LoopWin::draw` function, which can be defined using a lambda-expression.
```C++
int main() 
{
   Window window { <Title in top bar>, <width>, <height>};
   Circle circle { 0, 0, 100 };
   window.attach(Circle);

   window.draw = [](float frametime) {
      // user defined function
      // ...
      // change position, size, color of shapes 
      //   according to game/animation
   };

   window.run(); // don't forget!
   
   return 0;
}
```

##### Methods & fields
See above: `fbg::Window` methods are inhertied, and therefore accessible
```C++
*ALL METHODS OF fbg::Window are usable*

// user defined stored functions:
std::function<void(float dt)> draw;       // called before frame is rendered in run() loop
                                          //  - gives frametime of last frame as a parameter (dt)
std::function<void(float dt)> after_draw; // called after frame is rendered in run() loop
std::function<void(void)>     before_run; // called right after Window::run is called.

// Methods:
void  LoopWin::framerate(float f); // setter
float LoopWin::framerate() const;  // getter

int   LoopWin::framesElapsed() const; // returns number of frames completed since window was created

void  LoopWin::run()                  // Run gameloop. Stops execution until window closes. */
```

### Shapes

#### `fbg::Rect`: The rectangle class


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

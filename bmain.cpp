#include "2D_Graphics_Lib.hpp"
#include <array>

#include "boid.hpp"

using namespace fbg;

int main() {
   LoopWin win { "Boids flocking simulation.", 1280, 720 };

   win.background(0, 12);
   win.framerate(144);

   Flock flock { win, 140 };

   win.draw = [&](float dt) {
      flock.update(dt);
   };

   win.run();

   log_window_performance(win);
   
   return 0;
}
#include "fbg.hpp"
#include <array>

#include "boid.hpp"

using namespace fbg;


int main() {
   LoopWin win { "Boids flocking simulation", 2560, 1200 };

   win.background(0, 40);
   win.framerate(60);

   Flock flock { win, 1000 };

   Rect rect { 0, 0, 100, 100 };

   win.draw = [&](float dt) {

      flock.update(dt);

   };

   win.run();

   log_window_performance(win);
   
   return 0;
}
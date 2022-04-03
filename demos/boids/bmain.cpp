#include "2D_Graphics_Lib.hpp"
#include <array>

#include "boid.hpp"

using namespace fbg;


int main() {
   LoopWin win { "Boids flocking simulation.", 640, 480 };

   win.background(52);
   win.framerate(600);

   Flock flock { win, 140 };

   Rect rect { 0, 0, 100, 100 };

   win.draw = [&](float dt) {

      flock.update(dt);

   };

   win.run();

   log_window_performance(win);
   
   return 0;
}
#include "fbg.hpp"

using namespace fbg;

int main() 
{  
   LoopWin win { "Test of alpha background", 480, 480 };

   win.background(0, 20);
   win.framerate(144);

   V2d<float> middle = win.dimensions() / 2;
   
   Rect rect { middle, 100, 100 };

   rect.strokeweight(5);
   rect.stroke(255);
   rect.noFill();

   win.attach(rect);

   float x = 0;
   win.draw = [&] (float dt) {
      rect.rotate(dt * 3.0f);
      rect.width(sinf(x) * 150.0f + 170.0f);
      rect.height(sinf(x) * 150.0f + 170.0f);
      x += dt;
   };

   win.run();

}
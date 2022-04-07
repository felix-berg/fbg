#include "fbg.hpp"

using namespace fbg;

int main()
{
   LoopWin win { "Transparency example", 800, 800 };

   Rect grr { win.dimensions() / 2, 300, 300 };
   Rect rdr { win.dimensions() / 2 - V2d<float> { 100, 100 }, 120, 220 };
   Rect blr { win.dimensions() / 2 + V2d<float> { 100, 100 }, 234, 140 };
   Rect mouser { 0, 0, 200, 200 };

   // attach to screen   -  priorities
   win.attach(grr, rdr);     // 4 (behind)
   // win.attach(rdr);     // 3
   win.attach(blr);     // 2
   win.attach(mouser);  // 1 (frond)

   win.background(255);
   grr.fill(123, 230, 100, 140);
   rdr.fill(255, 0, 120, 120);
   blr.fill(0, 0, 0, 180);
   mouser.fill(0, 60, 255, 50);

   win.draw = [&](float) {
      mouser.pos(win.mouse());
      mouser.rotate(0.01f);
      rdr.rotate(0.02f);
      blr.rotate(0.03f);
   };

   win.run();

   return 0;
}
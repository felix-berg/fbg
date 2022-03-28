#include "2D_Graphics_Lib.hpp"
#include <array>

using namespace fbg;

int main() {
   LoopWin win { "Test of simple shapes.", 1280, 720 };

   win.background(52);
   win.framerate(144);

   Context context { win.dimensions() / 2.0f };

   auto rs = [&]() -> V2d<float> {
      return std::move(random_vector(-win.width(), win.width(), -win.height(), win.height()));
   };

   std::array<Rect, 50> rects;
   for (Rect & r : rects) {
      r.pos(rs());
      r.width(20); r.height(20);
      r.fill(255);

      context.attach(r);
   }

   std::array<Circle, 50> circles;
   for (Circle & c : circles) {
      c.pos(rs());
      c.radius(10);
      c.fill(255);

      context.attach(c);
   }

   std::array<Line, 50> lines;
   for (Line & l : lines) {
      l.from(rs());
      l.to(rs());
      l.strokeweight(1);
      
      context.attach(l);
   }

   std::array<Triangle, 50> triangles;

   for (Triangle & t : triangles) {
      V2d<float> p   = rs();
      V2d<float> off { 0.0f, 20.0f };
      t.point(0, p + off);
      off.rotate((120.0f / 360.0f) * twoPi);
      t.point(1, p + off);
      off.rotate((120.0f / 360.0f) * twoPi);
      t.point(2, p + off);

      context.attach(t);
   }

   win.attach(context);

   float t = 0;
   win.draw = [&](float dt) -> void {
     context.rotate(0.001f);
     context.origin(win.mouse());
   };
   
   win.run();

   log_window_performance(win);
   

   context.move(20, 20);

   return 0;
}
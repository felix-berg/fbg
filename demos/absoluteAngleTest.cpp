#include "2D_Graphics_Lib.hpp"
#include <array>

using namespace fbg;

int main() {
   LoopWin win { "Test of simple shapes.", 640, 480 };

   win.background(52);
   win.framerate(1000000);

   Context context { win.dimensions() / 2.0f };

   auto rs = [&]() -> V2d<float> {
      return std::move(random_vector(-win.width(), win.width(), -win.height(), win.height()));
   };

   Rect::MODE = Rect::DrawMode::CORNER;

   std::array<Rect, 50> rects;
   for (Rect & r : rects) {
      r.pos(rs());
      r.width(20); r.height(20);
      r.fill(rand() % 255, rand() % 255, rand() % 255, rand() % 255);

      context.attach(r);
   }

   std::array<Circle, 50> circles;
   for (Circle & c : circles) {
      c.pos(rs());
      c.radius(10);
      c.fill(rand() % 255, rand() % 255, rand() % 255, rand() % 255);

      context.attach(c);
   }

   std::array<Line, 50> lines;
   for (Line & l : lines) {
      l.from(rs());
      l.to(rs());
      l.strokeweight(1);
      l.stroke(rand() % 255, rand() % 255, rand() % 255, rand() % 255);
      
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

      t.fill(rand() % 255, rand() % 255, rand() % 255, rand() % 255);

      context.attach(t);
   }

   std::array<Polyline, 50> polylines;

   for (Polyline & pl : polylines) {
      V2d<float> p   { rs() };
      V2d<float> off { 0.0f, 15.0f };
      int numEdges = rand() % 8 + 4;
      
      for (int i = 0; i < numEdges; i++) {
         off.rotate(twoPi / float(numEdges - 1));
         pl.vertex(p + off);
      }

      pl.fill(rand() % 255, rand() % 255, rand() % 255, rand() % 255);

      context.attach(pl);
   }


   Image img { rs() / 2.0f, "test2.bmp"};
   context.attach(img);
   
   win.attach(context);

   float t = 0;
   win.draw = [&](float dt) -> void {
      context.angle((context.origin() - win.mouse()).angle());
   };
   
   win.run();

   log_window_performance(win);
   
   context.move(20, 20);

   return 0;
}

#include "fbg.hpp"
#include <cmath>

using namespace fbg;

void update_times_table(std::vector<Line> & lines, const Circle & circle, float times)
{
   float anglePer = twoPi / float(lines.size());

   for (int i = 0; i < lines.size(); i++) {
      int connection = static_cast<int> (std::floor(float(i) * times)) % lines.size();

      lines[i].from(
         circle.pos().x + std::cos(anglePer * i) * circle.radius(),
         circle.pos().y + std::sin(anglePer * i) * circle.radius()
      );

      lines[i].to(
         circle.pos().x + std::cos(anglePer * connection) * circle.radius(),
         circle.pos().y + std::sin(anglePer * connection) * circle.radius()
      );

      lines[i].strokeweight(1);
      lines[i].roughEdge();
      lines[i].stroke(0);
   }
}

int main() 
{
   LoopWin win { "Times table cardiod animation", 640, 640 };

   win.framerate(144);

   int numPoints = 500;
   float times = 0.0f;

   Circle circle { win.dimensions() / 2, win.height() * 0.4f };

   circle.strokeweight(3);
   circle.noFill();

   win.attach(circle);

   std::vector<Line> lines (numPoints);
   for (Line & l : lines)
      win.attach(l);

   win.draw = [&](float dt) {
      update_times_table(lines, circle, times);
      times += dt * 0.5f;
   };
   
   win.run();

   fbg::log_window_performance(win);

   return 0;

}
#include "fbg.hpp"

using namespace fbg;

class Aeroplane : public Context {
public:
   float maxSteerForce = 400.0f;
   float maxSpeed      = 2000.0f;

   V2d<float> vel {},
              acc {};

   Aeroplane(float x, float y) 
      : Context { x, y }
   {
      cockpit.noStroke();
      window.fill(100, 255, 100);

      body.fill(255);

      this->attach(cockpit);
      this->attach(window);
      this->attach(lwing);
      this->attach(rwing);
      this->attach(body);
   }

   void seek_velocity(const V2d<float> & desiredVel) {
      V2d<float> steerAcc = desiredVel - vel;

      steerAcc.size(maxSteerForce);

      acc += steerAcc;
   }

   void seek_point(const V2d<float> & point) {
      V2d<float> desiredVel = point - origin();
      float d = desiredVel.size();

      seek_velocity(desiredVel);
   }

   void update(float dt) {
      vel += acc * dt;
      vel.limit(maxSpeed);
      this->move(vel * dt);
      acc = {0, 0};

      this->angle(vel.angle() - halfPi);
   }

private:
   Circle cockpit { 0, 20, 6 };
   Triangle window {
      { -4, 20 },
      {  4, 20 },
      {  0, 24 }
   };

   Triangle lwing {
      {   0, -6 },
      { -60, -6 },
      {   0,  6 }
   };

   Triangle rwing {
      {  0, -6 },
      { 60, -6 },
      {  0,  6 }
   };

   Polyline body { 
      {  6,  20 }, 
      {  6, -35 },
      {  3, -40 },
      { -3, -40 },
      { -6, -35 },
      { -6,  20 }
   };


};

int main() 
{
   LoopWin win { "Custom shapes with Context class example", 1280, 720 };

   Aeroplane ap { (float) win.width() / 2, (float) win.height() / 2 };
   
   win.attach(ap);

   win.draw = [&](float dt) {
      ap.seek_point(win.mouse());
      ap.update(dt);
   };

   win.run();

   return 0;
}
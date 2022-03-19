#include "2D_Graphics_Lib.hpp"
#include <array>


using namespace fbg;

class Particle : public Circle {
public:
	float mass;

	Particle(float x, float y, float r) : Circle { x, y, r } {
		mass = r / 1000.0f;

		stroke(0);
		fill(255);
	};

	Particle() : Particle { 0.0f, 0.0f, 10.0f } { };

	void apply_force(V2d<float> acc) {
		a += acc;
	}

	void update(int width, int height) {
		v += a;
		this->pos(this->pos() + v);

		a.x = 0.0f; a.y = 0.0f;

		V2d<float> p = pos();
		if (p.x < 0.0f) 	 p.x = width - 1.0f;
		if (p.x >= width)  p.x = 0.0f;
		if (p.y < 0.0f)	 p.y = height - 1.0f;
		if (p.y >= height) p.y = 0.0f;
		pos(p);
	}

private:
	V2d<float> v, a;
};

int main() {
	srand(clock());
	fbg::LoopWin win { "Window", 1280, 720 };

	win.framerate(144.0f);
	win.background(140, 20);

	std::array<Rect, 4> particles;

	for (auto & p : particles) {
		p.pos(win.dimensions() / 2);
		p.width(100);
		p.height(100);
		win.attach(p);
	}

	win.before_draw = [&](float dt) -> void {
		


	};

	win.run();
}
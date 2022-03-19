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
		no_stroke();
	};

	Particle() : Particle { 0.0f, 0.0f, 5.0f } { };

	void apply_force(V2d<float> f) {
		a += f / mass;
	}

	void accelerate_towards(V2d<float> p) {
		V2d<float> dir = p.normal();
		this->apply_force(dir / p.size_sq());
	}

	void update(float dt, int width, int height) {
		v += a * dt;
		a.x = 0.0f; a.y = 0.0f;
		this->pos(pos() + v);


		V2d<float> p = pos();
		if (p.x < 0.0f) 	 p.x = width - 1.0f + p.x;
		if (p.x >= width)  p.x = p.x - width;
		if (p.y < 0.0f)	 p.y = height - 1.0f + p.y;
		if (p.y >= height) p.y = p.y - height;
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

	std::array<Particle, 100> particles;

	for (auto & p : particles) {
		p.pos(win.dimensions() / 2);

		p.apply_force(random_vector(1.0f, 1.0f));
		
		win.attach(p);
	}

	win.before_draw = [&](float dt) -> void {
		if (win.mouse_down())
		for (auto & p : particles)
		for (auto & p : particles) {
			p.update(dt, win.width(), win.height());
		}
	};

	win.run();
}
#include <array>
#include "2D_Graphics_Lib.hpp"

template <int nx, int ny>
struct VectorGrid {
	const int cell_sz;

	std::array<V2d<float>, nx * ny> vectors;
	std::array<Line, 		  nx * ny> lines;
	std::array<Rect,  nx * ny> rects;

	VectorGrid(int cell_size) : cell_sz { cell_size } { 
		for (int y = 0; y < ny; y++)
			for (int x = 0; x < nx; x++) {
				int tx = x * cell_sz;
				int ty = y * cell_sz;

				Rect & r = rects[y * nx + x];
				r.pos(tx, ty);
				r.stroke(255); 
				r.fill(0);
				r.width(cell_sz);
				r.height(cell_sz);

				vectors[y * nx + x] = random_vector(1.0f, 1.0f);

				Line & l = lines[y * nx + x];
				l.from(tx + cell_sz / 2, ty + cell_sz / 2);
				l.to(l.from() + vectors[y * nx + x] * double(cell_size) * 0.5);
			}
	};

	template <typename WDW> 
	void attach_to(WDW & window) { 
		// for (Rectangle & r : rects)
		// 	window.attach(r);
		for (Line & l : lines)
			window.attach(l);
	}

	V2d<float> velocity_at(V2d<float> p) {
		int idx = int(p.y / cell_sz) * nx + int(p.x / cell_sz);

		if (idx >= nx * ny || idx < 0) // ob
			return {0.0f, 0.0f};

		return vectors[idx];
	}
};

struct Particle {
	V2d<float> p, v {0.0f, 0.0f}, 
				  a {0.0f, 0.0f};
	Circle circle;

	Particle(V2d<float> pt, float r) : p { pt },  circle { V2d<int>(p), int(r) } { }

	/* Convinience constructors */
	Particle(float x, float y, float r) : Particle { {x, y}, r } { }
	Particle() : Particle { 0, 0, 0 } { };

	void apply_force(const V2d<float> & f) {
		a += f;
	}

	void update(int w, int h) {
		v += a;
		p += v;
		a = {0.0f, 0.0f};

		if (p.x < 0)  p.x = w - 1;
		if (p.x >= w) p.x = 0;
		if (p.y < 0)  p.y = h - 1;
		if (p.y >= h) p.y = 0;

		circle.pos(p);
	}
};

int main() {
	srand(clock());
	LoopWin win { "Test Window", 1280, 720 };
	win.background({0, 0, 0, 20});
	win.framerate(144.0f);

	VectorGrid<1280 / 20, 720 / 20> vg { 20 };
	std::array<Particle, 50> particles; 
	vg.attach_to(win);

	for (Particle & p : particles) {
		p.p = {fbpm::random(0.0f, (float) win.width()),fbpm::random(0.0f,  (float) win.height())};
		p.circle.radius(5);
		win.attach(p.circle);
	}

	// definition of game loop -> called every frame.
	win.before_draw = [&](float dt) -> void {
		V2d<int> mouse = win.mouse_pos_clamped();
		for (Particle & p : particles) {
			p.apply_force(vg.velocity_at(p.p) * dt);
			p.update(win.width(), win.height());
		}
	};

	win.run(); // waits here until window is closed

	return 0;
}
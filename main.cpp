#include <SDL2/SDL.h>

#include <iostream>
#include <vector>
#include <array>

#include "Graphics_Lib/Shapes/shape.hpp"
#include "Graphics_Lib/Shapes/line.hpp"
#include "Graphics_Lib/Shapes/rect.hpp"
#include "Graphics_Lib/Shapes/context.hpp"
#include "Graphics_Lib/Window.hpp"
#include "Graphics_Lib/alphacomposite.hpp"

void test_alpha_compositing();
void test_alpha_compositing2();

int main() {
	srand(clock());
	Window window {"uh", 1080, 720};
	window.background({0, 0, 0, 1});
	

	std::array<Rectangle, 1> rects;
	std::array<V2d<float>, 1> positions;

	for (Rectangle & r : rects) {
		r.stroke(255);
		r.pos(rand() % window.width(), rand() % window.height());
		r.width(50);
		r.height(50);
		window.attach(r);
	}

	for (V2d<float> & v : positions)
		v = {float(rand() % window.width()), float(rand() % window.height())};

	int total_frames = 0; clock_t start_time = clock();
	while (window.is_open() && !window.is_key_pressed(SDL_SCANCODE_ESCAPE)) {
		V2d<int> mouse = window.get_mouse_pos();

		for (int i = 0; i < rects.size(); i++)  {
			positions[i] += (V2d<float> {float(mouse.x), float(mouse.y)} - positions[i]) * (float(rand() % 100000) / 10000000.0f);
			rects[i].pos(positions[i].x, positions[i].y);
		}

		window.draw();
		total_frames++;
	}

	clock_t total_time = clock() - start_time;
	float total_time_mcs = (float) total_time / (float) total_frames;
	std::cout << "Average frametime: " << total_time_mcs << " µs\n";
	std::cout << "Average framerate: " << 10e5 / total_time_mcs << " fps\n";

	return 0;
}

void test_alpha_compositing2() {
	srand(clock());
	Rgba * a = (Rgba *) malloc(sizeof(Rgba) * 196000);
	Rgba * b = (Rgba *) malloc(sizeof(Rgba) * 196000);

	for (int i = 0; i < 196000; i++) {
		// a[i] = {clock() % 255, clock() % 255, clock() % 255, clock() % 255};
		// b[i] = {clock() % 255, clock() % 255, clock() % 255, clock() % 155};
	}

	clock_t start, stop;
	start = clock();
	for (int i = 0; i < 196000; i += 8)
		alpha_composite8(a + i, b + i);


	stop = clock();
	std::cout << stop - start << '\n';

	start = clock();
	// for (int i = 0; i < 196000; i += 8)
	// 	alpha_composite8(a + i, b + i);

	for (int i = 0; i < 196000; i++)
		alpha_composite1(a + i, b + i);

	stop = clock();
	std::cout << stop - start << '\n';
}

// void test_alpha_compositing() {

// 	Rgba * a = (Rgba *) malloc(sizeof(Rgba) * 196000);
// 	Rgba * b = (Rgba *) malloc(sizeof(Rgba) * 196000);

// 	std::cout << "initializeing...\n"; 

// 	for (int i = 0; i < 196000; i++) {
// 		a[i] = {121, 2, 3, 4};
// 		b[i] = {5, 100, 100, 200};
// 	}

// 	clock_t start_ = clock(), end_;

// 	for (int i = 0; i < 196000; i += 32)
// 		alpha_composit_16(a + i, b + i);
	
// 	end_ = clock();

// 	std::cout << "It took: " << end_ - start_ << " µs\n";

// 	std::cout << "Initializing...\n";
// 	for (int i = 0; i < 196000; i++) {
// 		a[i] = {121, 2, 3, 4};
// 		b[i] = {5, 100, 100, 200};
// 	}

// 	start_ = clock();

// 	for (int i = 0; i < 196000; i++) {
// 		alpha_composit_1(&a[i], &b[i]);
// 	}
	
// 	end_ = clock();

// 	std::cout << "It took: " << end_ - start_ << " µs\n";
// }
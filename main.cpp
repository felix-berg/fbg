#include <SDL2/SDL.h>

#include <iostream>
#include <vector>
#include <array>

#include "Graphics_Lib/Shapes/shape.hpp"
#include "Graphics_Lib/Shapes/line.hpp"
#include "Graphics_Lib/Shapes/context.hpp"
#include "Graphics_Lib/Window.hpp"
#include "Graphics_Lib/alphacomposit.hpp"

void test_alpha_compositing();
void test_alpha_compositing2();

int main() {
	srand(clock());
	Window window {"uh", 640, 480};
	window.background({255, 255, 255, 254});
	
	std::array<Line, 100> lines;
	for (Line & l : lines) l.stroke({0, 0, 0, 255});

	for (Line & l : lines) window.context.attach(l);
	for (Line & l : lines) l.from({rand() % window.width(), rand() % window.height()});

	int total_frames = 0;

	clock_t start_time = clock();

	while (window.is_open() && !window.is_key_pressed(SDL_SCANCODE_ESCAPE)) {
		for (Line & l: lines) l.to(window.get_mouse_pos());
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
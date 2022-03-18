#include <SDL2/SDL.h>

#include <iostream>
#include <vector>
#include <array>
#include <functional>

#include "2D_Graphics_Lib.hpp"

int main() {
	srand(clock());
	Window window {"uh", 1080, 720};
	window.background(0, 50);
	
	std::array<Circle, 100> circles;
	for (Circle & c : circles) {
		c.pos(rand() % window.width(), rand() % window.height());
		window.attach(c);
	}	

	int total_frames = 0; clock_t start_time = clock();
	while (window.is_open() && !window.is_key_pressed(SDL_SCANCODE_ESCAPE)) {
		V2d<int> mouse = window.get_mouse_pos();

		for (Circle & c : circles)
			c.radius((mouse - c.pos()).size());

		window.draw();
		total_frames++;
	}

	clock_t total_time = clock() - start_time;
	float total_time_mcs = (float) total_time / (float) total_frames;
	std::cout << "Average frametime: " << total_time_mcs << " µs\n";
	std::cout << "Average framerate: " << 10e5 / total_time_mcs << " fps\n";

	return 0;
}
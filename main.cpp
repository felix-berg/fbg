#include <SDL2/SDL.h>

#include <iostream>
#include <vector>
#include <array>
#include <functional>

#include "2D_Graphics_Lib.hpp"

int main() {
	srand(clock());
	Window window {"uh", 1080, 720};
	window.background(0, 2);
	
	Circle c {{200, 200}, 5};
	c.stroke({255, 0, 0, 255});

	window.attach(c);

	int total_frames = 0; clock_t start_time = clock();
	while (window.is_open() && !window.is_key_pressed(SDL_SCANCODE_ESCAPE)) {
		V2d<int> mouse = window.get_mouse_pos();

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
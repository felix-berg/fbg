#include <SDL2/SDL.h>

#include <iostream>
#include <vector>
#include <functional>

#include "Graphics_Lib/Shapes/shape.hpp"
#include "Graphics_Lib/Shapes/line.hpp"
#include "Graphics_Lib/Shapes/context.hpp"
#include "Graphics_Lib/Window.hpp"

int main() {
	Window window {"uh", 800, 800};
	window.background({0, 0, 0, 0});
	
	Line l {{100, 100}, window.get_mouse_pos()};

	window.context.attach(l);

	int total_frames = 0;

	clock_t start_time = clock();

	while (window.is_open() && !window.is_key_pressed(SDL_SCANCODE_ESCAPE)) {
		l.to(window.get_mouse_pos());
		window.draw(); 
		total_frames++;
	}

	clock_t total_time = clock() - start_time;
	float total_time_secs = (float) total_time / (float) CLOCKS_PER_SEC;
	
	std::cout << "Average framerate: " << (float) total_frames / total_time_secs << " fps\n";
	
	return 0;
}
#include <SDL2/SDL.h>

#include <iostream>
#include <vector>
#include <functional>

#include "Graphics_Lib/Shapes/shape.hpp"
#include "Graphics_Lib/Shapes/line.hpp"
#include "Graphics_Lib/Shapes/context.hpp"
#include "Graphics_Lib/Window.hpp"

int main() {
	srand(clock());
	Window window {"uh", 1000, 800};
	window.background(0, 100);
	
	std::vector<Line> lines { 100 };
	for (Line & l : lines) l.stroke({255, 255, 255, 255});

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
	float total_time_secs = (float) total_time / (float) CLOCKS_PER_SEC;
	
	std::cout << "Average framerate: " << (float) total_frames / total_time_secs << " fps\n";
	
	return 0;
}
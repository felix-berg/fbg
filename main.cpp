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

int main() {
	srand(clock());
	Window window {"uh", 1080, 720};
	window.background({0, 0, 0, 255});
	
	std::array<Line, 100> lines;
	
	for (Line & l : lines) {
		l.stroke(255);
		l.from(rand() % window.width(), rand() % window.height());
		window.attach(l);
	}

	int total_frames = 0; clock_t start_time = clock();
	while (window.is_open() && !window.is_key_pressed(SDL_SCANCODE_ESCAPE)) {
		V2d<int> mouse = window.get_mouse_pos();

		for (Line & l: lines)
			l.to(mouse.x, mouse.y);

		window.draw();
		total_frames++;
	}

	clock_t total_time = clock() - start_time;
	float total_time_mcs = (float) total_time / (float) total_frames;
	std::cout << "Average frametime: " << total_time_mcs << " µs\n";
	std::cout << "Average framerate: " << 10e5 / total_time_mcs << " fps\n";

	return 0;
}
#include <SDL2/SDL.h>

#include <iostream>
#include <vector>
#include <array>

#include "Graphics_Lib/Shapes/shape.hpp"
#include "Graphics_Lib/Shapes/line.hpp"
#include "Graphics_Lib/Shapes/point.hpp"
#include "Graphics_Lib/Shapes/circle.hpp"
#include "Graphics_Lib/Shapes/rect.hpp"
#include "Graphics_Lib/Shapes/context.hpp"
#include "Graphics_Lib/Window.hpp"
#include "Graphics_Lib/alphacomposite.hpp"

int main() {
	srand(clock());
	Window window {"uh", 1080, 720};
	window.background(0, 255);
	
	std::array<Circle, 100> circles;
	std::array<Point, 100 > points;

	for (int i = 0; i < circles.size(); i++) {
		Circle & c = circles[i];
		c.pos(rand() % window.width(), rand() % window.height());
		points[i].pos(c.pos());
		c.radius(100);
		window.attach(c);
		window.attach(points[i]);
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
#include <SDL2/SDL.h>

#include <iostream>
#include <vector>
#include <functional>

#include "Graphics_Lib/Shapes/shape.hpp"
#include "Graphics_Lib/Shapes/rect.hpp"
#include "Graphics_Lib/Shapes/line.hpp"
#include "Graphics_Lib/Window.hpp"

void window_loop(Window & win, std::function<void ()> before_render) {
	
}


int main() {
	Window win;

	srand(clock());

	const int num_lines = win.height();
	Line lines[num_lines];


	for (int i = 0; i < num_lines; i++) {
		lines[i].set_from({0, i});
		lines[i].set_to({win.width(), i});
		win.attach(lines[i]);
	}

	unsigned long long time_sum = 0, frames = 0;

	while (win.is_open()) {
		unsigned long long start_t = clock();
		Rgba bck_col = {255, 255, 0, 255};

		if (win.is_key_pressed(SDL_SCANCODE_ESCAPE))
			break;

		win.draw();
		time_sum += clock() - start_t;
		win.detach(lines[frames]);
		if (frames >= num_lines) break;
		frames++;
	}


	double avg_frametime = (double) time_sum / (double) frames;
	double avg_framerate = CLOCKS_PER_SEC / avg_frametime;
	std::cout << "Total number of frames elapsed: " << frames << '\n';
	std::cout << "Total time: " << (double) time_sum / CLOCKS_PER_SEC << " seconds \n";
	std::cout << "Average frametime: " << avg_frametime << " µs \n";
	std::cout << "Average framerate: " << avg_framerate << " fps \n";

	return 0;
}
#include <SDL2/SDL.h>

#include <iostream>
#include <vector>
#include <array>
#include <functional>

#include "2D_Graphics_Lib.hpp"

int main() {
	LoopWin win { "Test Window", 1280, 720 };
	win.background({0, 0, 0, 20});
	win.framerate(144.0f);

	// definition of game loop -> called every frame.
	win.before_draw = [&](int dt) -> void {
		
	};

	win.run(); // sits here until window is closed
	return 0;
}
#include "2D_Graphics_Lib.hpp"

int main() {
	// Construct 1280x720 Window object.
	fbg::LoopWin window { "Window Title", 1280, 720 };

	// set window attributes:
	window.background(0, 20); // black with alpha 20
	window.framerate(144.0f);
	
	// Create circle object at (0, 0) with radius 50
	fbg::Circle circle {0.0f, 0.0f, 50.0f};
	
	// draw circle object at every frame update,
	// by attaching it to the window object
	window.attach(circle);

	// define function that will be called at every frame
	window.before_draw = [&](float frametime) -> void {
		// grab the mouse position from the window
		fbg::V2d<int> mpos = window.mouse();

	
		// set the position of the radius to the
		// current mouse position
		circle.pos(mpos);
	};

	// Start the game loop by calling run.
	window.run(); 

	return 0; // :)
}

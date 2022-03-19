#include "2D_Graphics_Lib.hpp"

int main() {
	// Construct 1280x720 Window object.
	LoopWin window { "Window Title", 1280, 720 };

	// set window attributes:
	window.background(0, 20);
	window.framerate(144);
	
	// Create circle object at (0, 0) with radius 50
	Circle circle {0, 0, 50};
	
	// draw circle object at every frame update,
	// by attaching it to the window object
	window.attach(circle);

	// define function that will be called at every frame
	window.before_draw = [&](float frametime) -> void {
		// grab the mouse position from the window
		V2d<int> mpos = window.get_mouse_pos();

		// set the position of the radius to the
		// current mouse position
		circle.pos(mpos);
	};

	// Start the game loop by calling run.
	window.run(); 

	return 0; // :)
}
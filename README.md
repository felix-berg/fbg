# Low-level 2D Graphics Library!



## Simple sketch
```C++
#include "2D_Graphics_Lib.hpp"

using namespace fbg;

int main() {
	// Construct 1280x720 Window object.
	LoopWin window { "Window Title", 1280, 720 };

	// set window attributes:
	window.background(0, 50); // set background color to black with alpha 50
	window.framerate(112.0f);
	
	// Create circle object at (0, 0) with radius 50
	Circle circle {0.0f, 0.0f, 50.0f};
	
	// draw circle object at every Frame update,
	// by attaching it to the window object
	window.attach(circle);

	// define function that will be called at every frame
	window.draw = [&](float frametime) -> void {
		// grab the mouse position from the window
		V2d<int> mpos = window.mouse();
	
		// set the position of the radius to the
		// current mouse position
		circle.pos(mpos);
	};

	// Start the game loop by calling run.
	window.run(); 

	return 0; // :)
}
```

Produces output:
![Final output](https://user-images.githubusercontent.com/93908883/159099446-f5976859-6240-45c8-a9e5-a05dad4b5cb2.png)


## Particle-system: Bouncy-balls
The following example produces the following output:
![image](https://user-images.githubusercontent.com/93908883/161541847-820393f2-ca34-48e9-9ac0-7109823e78f2.png)
The example shows how the fbg::Shape derived classes can be expanded upon, to simplify graphics programming.

#include "2D_Graphics_Lib.hpp"

int main() {
	LoopWin win { "Window", 1280, 720 };

	Line l {{fbpm::random(win.width()), fbpm::random(win.height())}, {0, 0}};
	Rect r {10, 10, 200, 200};

	win.attach(l);
	win.attach(r);
	r.stroke({230, 0, 0, 255});
	l.stroke({230, 0, 0, 255});
	r.fill({120, 255, 0, 255});
	
	l.strokeweight(6);
	r.strokeweight(5);

	win.after_draw = [&](float dt) -> void {
		l.to(win.mouse());
	};

	win.run();
}
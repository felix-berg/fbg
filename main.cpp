#include "2D_Graphics_Lib.hpp"

int main() {
	fbg::LoopWin w;

	fbg::Circle c { 100.0f, 100.0f, 50.0f};

	w.attach(c);
	
	
	w.before_draw = [&](float dt) {
		c.pos(w.mouse());
	};

	w.run();
	fbg::log_window_performance(w);
}
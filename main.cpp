#include "2D_Graphics_Lib.hpp"

int main() {
	fbg::LoopWin w;

	fbg::Circle c { 100.0f, 100.0f, 50.0f};

	

	w.attach(c);
	w.run();
}
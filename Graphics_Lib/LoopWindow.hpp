#ifndef LWINDOW_HPP
#define LWINDOW_HPP

#include <functional>
#include <thread>
#include <chrono>

#include "Window.hpp"

using std::chrono::high_resolution_clock;
namespace fbg {

/* Class for running animations inside a Window */
class LoopWin : public Window {
public:
	/* Constructor for looping window */
	LoopWin(const std::string & title, int w, int h) : Window { title, w, h } { };

	/* Default constructor: Applies sensible defaults */
	LoopWin() : LoopWin { "Looping Window", 640, 480 } { };
	
	/*
		This function is called immediately before the
		loop starts in run().
	*/
	std::function<void()> before_run = nullptr;

	/* This function is called immediately before the
		window::draw() method is called.
		Provides the expected frametime in seconds. */
	std::function<void(float)> before_draw = nullptr;

	/* Same as before_draw, but after. */
	std::function<void(float)> after_draw = nullptr;

	// getter and setter for the framerate 
	float framerate() const { 
		float dt = frametime();
		return 1.0f / dt;  // return 1.0f / frametime
	};

	void framerate(float f)   { 
		// convert floating point framerate into microseconds in 64 bit integer format
		m_frametime = std::chrono::duration_cast<std::chrono::duration<int, std::micro>> (std::chrono::duration<float, std::micro> (1000000.0f / f));
	};

	float frametime() const {
		// convert frametime from microseconds 64 int -> seconds in float
		return std::chrono::duration_cast<std::chrono::duration<float, std::ratio<1, 1>>> (m_frametime).count(); // why
	}

	/* Return number of frames completed since run() was called. */
	int frames_elapsed() const { return m_num_frames; };
	float total_time() const { 
		if (is_open()) throw std::runtime_error("Cannot get total time while window is running.");
		return m_total_time.count();
 	}

	/*
		Run gameloop. Stop execution until window closes.
	*/
	void run() { 
		// user defined function
		if (before_run != nullptr)
			before_run();

		m_start_time = high_resolution_clock::now();
		m_last_frame = m_start_time;
		
		std::chrono::duration<float, std::ratio<1>> dt = m_frametime; // time for last frame. initialized as expected framerate
		while (is_open()) {
			high_resolution_clock::time_point frame_start_time = high_resolution_clock::now();

			// update timer
			m_last_frame += m_frametime;
			
			// wait for enough time to have passed
			std::this_thread::sleep_until(m_last_frame);

			// user defined function
			if (before_draw != nullptr) 
				before_draw(dt.count());

			// draw frame
			draw();
			m_num_frames++;

			if (after_draw != nullptr)
				after_draw(dt.count());

			dt = std::chrono::duration_cast<std::chrono::duration<float, std::ratio<1, 1>>>(high_resolution_clock::now() - frame_start_time);
		}
		m_total_time = high_resolution_clock::now() - m_start_time;
	}

private:
	std::chrono::microseconds m_frametime { 1000000 / 60 }; // time per frame in µs
	std::chrono::duration<float, std::ratio<1, 1>> m_total_time;

	high_resolution_clock::time_point m_last_frame, m_start_time;

	int m_num_frames = 0; // total number of frames elapsed
};
};

#endif
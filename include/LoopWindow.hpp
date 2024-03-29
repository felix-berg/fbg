#ifndef LWINDOW_HPP
#define LWINDOW_HPP

#include <iostream>
#include <functional>
#include <thread>
#include <chrono>

#include "Window.hpp"

using std::chrono::high_resolution_clock;

namespace fbg {

/** Class for running animations inside a Window.
 * @param draw(): Function pointer, that can be defined. Called immediately before every Frame is rendered.
 * @param after_draw(): Function pointer, that can be defined. Called immediately after every Frame is rendered. 
 * @param run(): Function that starts rendering. Stops execution until window is closed.
 * @param framerate(): Getter/setter for framerate. */
   class LoopWin : public Window {
   public:
      constexpr static float Unlimited = INT16_MAX;   
      struct InvalidFramerate : public std::runtime_error {
         InvalidFramerate(float f)
            : std::runtime_error { "Invalid framerate: " + std::to_string(f) } { };
      };

      /** Constructor for looping window. 
       * @param title: The title at the top bar of the window.
       * @param w: The width of the window.
       * @param h: The height of the window. */
      LoopWin(const std::string & title, int w, int h) : Window { title, w, h } { };

      /** Default constructor. 
       * Sets title to "Looping Window". Sets width to 640 px and height to 480 px. */
      LoopWin() : LoopWin { "Looping Window", 640, 480 } { };

      /** This function is called immediately before the	Window::update() method is called.
       * Provides the frametime of the previous frame	in seconds. */
      std::function<void(float)> draw = nullptr;

      /** This function is called immediately after the Window::update() method is called.
       * Provides the frametime of the previous frame	in seconds. */
      std::function<void(float)> after_draw = nullptr;

      /** This function is called immediately before the	Window::run() method starts the game loop. */
      std::function<void(void)> before_run = nullptr;

      /** Getter for framerate. 
       * @returns Current expected framerate of the window in frames/second */
      float framerate() const 
      { 
         float dt = frametime();
         return 1.0f / dt;  // return 1.0f / frametime
      };

      /** Setter for framerate. 
       * @param f: Framerate in frames/second */
      void framerate(float f)   
      { 
         if (f < 0.0f) throw InvalidFramerate(f);
         // convert floating point framerate into microseconds in 64 bit integer format
         m_frametime = std::chrono::duration_cast<std::chrono::duration<int, std::micro>> (std::chrono::duration<float, std::micro> (1000000.0f / f));
      };

      /** Getter for frametime. 
       * @returns Expected frametime of window. */
      float frametime() const 
      {
         // convert frametime from microseconds 64 int -> seconds in float
         return std::chrono::duration_cast<std::chrono::duration<float, std::ratio<1, 1>>> (m_frametime).count(); // why chrono
      }

      /** Getter for number of frames elapsed since LoopWin::run() was called.
       * @returns Number of frames completed since run() was called. */
      int framesElapsed() const { return m_numFrames; };
      
      /** Getter for total time since LoopWin::run() was called.
       * @returns Total amount of time in seconds. */
      float totalTime() const 
      { 
         if (isOpen()) 
            throw std::runtime_error("Cannot get total time while window is running.");
         return m_totalTime.count();
      }

      /** Run gameloop. Stops execution until window closes. */
      void run() 
      { 
         if (before_run != nullptr) before_run();

         m_startTime = high_resolution_clock::now();
         m_lastFrame = m_startTime;

         std::chrono::duration<float, std::ratio<1>> dt = m_frametime; // time for last frame. initialized as expected framerate
         while (isOpen()) {
            high_resolution_clock::time_point frameStartTime = high_resolution_clock::now();

            // update timer
            m_lastFrame += m_frametime;
            
            // wait for enough time to have passed
            std::this_thread::sleep_until(m_lastFrame);

            // user defined function
            if (draw != nullptr) 
               draw(dt.count());

            // draw shapes on frame
            Window::update();
            m_numFrames++;

            if (after_draw != nullptr)
               after_draw(dt.count());

            dt = std::chrono::duration_cast<std::chrono::duration<float, std::ratio<1, 1>>>(high_resolution_clock::now() - frameStartTime);
         }

         m_totalTime = high_resolution_clock::now() - m_startTime;
      }

   private:
      std::chrono::microseconds m_frametime { 1000000 / 60 }; // time per Frame in µs
      std::chrono::duration<float, std::ratio<1, 1>> m_totalTime;

      high_resolution_clock::time_point m_lastFrame, m_startTime;

      int m_numFrames = 0; // total number of frames elapsed

      bool m_isRunning = false;

      std::vector<int> m_closeKeys;
   };

   /** Log the expected and actual framerate of a window, that has finished execution.
    * @param win: The window to be analyzed. */
   void log_window_performance(const LoopWin & win) 
   {
      std::cout << "Expected framerate: " 
                  << win.framerate() << '\n';
      float frametime = win.totalTime() / static_cast<float>(win.framesElapsed());
      std::cout << "Actual framerate: "
                  << 1.0f / frametime << '\n';
      std::cout << '\n';
   } 
};

#endif
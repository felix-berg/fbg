#include "multithr_alphacomposite.hpp"

#include "rgba.hpp"
#include "alphacomposite.hpp"
#include <thread>
#include <mutex>
#include <atomic>
#include <condition_variable>
#include <vector>

#define THREADPOOL_SIZE 32
static std::atomic<int> number_of_finished_threads;

class Compositor_thread {
public:
	std::thread t;
	bool is_alive;
	std::mutex mutex_var;
	std::condition_variable cv;

	Rgba * ds, * ov;

	int start_n, n;

	void wait_for_start() {
		// run until death
		while (is_alive) {
			// wait for someone to screw with the lock
			std::unique_lock<std::mutex> lock(mutex_var);
			cv.wait(lock);

			// then composite
			alpha_compositeNC(ds + start_n, ov, n);

			// tell you are finished
			number_of_finished_threads++;
		}
	}

	~Compositor_thread() {
		is_alive = false;
		t.detach();
	}

	void composite(Rgba * dst, Rgba * over) {
		this->ds = dst;
		this->ov = over;
		cv.notify_one();
	}
};

Compositor_thread thread_pool[THREADPOOL_SIZE];

void init_compositor_threadpool(int scr_size) {
	int px_per_thread = scr_size / THREADPOOL_SIZE;

	for (int i = 0; i < THREADPOOL_SIZE; i++) {
		thread_pool[i].is_alive = true;
		thread_pool[i].start_n  = i * px_per_thread;
		thread_pool[i].n 		   = px_per_thread;
		thread_pool[i].t = std::thread(&Compositor_thread::wait_for_start, &thread_pool[i]);
	}
}

void multithr_alpha_compositeN(Rgba * dst, Rgba * over) {

}


void multithr_alpha_compositeNC(Rgba * dst, Rgba * over) {
	number_of_finished_threads = 0;
	for (int i = 0; i < THREADPOOL_SIZE; i++)
		thread_pool[i].composite(dst, over);
}
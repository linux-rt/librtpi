// { dg-options "-pthread" }
// { dg-do run { target { *-*-linux* *-*-gnu* } } }
// { dg-require-effective-target c++11 }
// { dg-require-effective-target pthread }
// { dg-require-gthreads "" }

#include "rtpi/condition_variable.hpp"
#include <chrono>
#include "rtpi/mutex.hpp"
#include <thread>

// PR libstdc++/103382

template<typename F>
void
test_cancel(F wait)
{
  rtpi::mutex m;
  rtpi::condition_variable cv;
  bool waiting = false;

  std::thread t([&] {
    std::unique_lock<rtpi::mutex> lock(m);
    waiting = true;
    wait(cv, lock); // __forced_unwind exception should not terminate process.
  });

  // Ensure the condition variable is waiting before we cancel.
  // This shouldn't be necessary because pthread_mutex_lock is not
  // a cancellation point, but no harm in making sure we test what
  // we intend to test: that cancel during a wait doesn't abort.
  while (true)
  {
    std::unique_lock<rtpi::mutex> lock(m);
    if (waiting)
      break;
  }

  pthread_cancel(t.native_handle());
  t.join();
}

int main()
{
  test_cancel(
      [](rtpi::condition_variable& cv, std::unique_lock<rtpi::mutex>& l) {
	cv.wait(l);
      });

  test_cancel(
      [](rtpi::condition_variable& cv, std::unique_lock<rtpi::mutex>& l) {
	cv.wait(l, []{ return false; });
      });

  using mins = std::chrono::minutes;

  test_cancel(
      [](rtpi::condition_variable& cv, std::unique_lock<rtpi::mutex>& l) {
	cv.wait_for(l, mins(1));
      });

  test_cancel(
      [](rtpi::condition_variable& cv, std::unique_lock<rtpi::mutex>& l) {
	cv.wait_until(l, std::chrono::system_clock::now() + mins(1));
      });
}

// { dg-do run }
// { dg-additional-options "-pthread" { target pthread } }
// { dg-require-effective-target c++11 }
// { dg-require-gthreads "" }

// Copyright (C) 2012-2022 Free Software Foundation, Inc.
//
// This file is part of the GNU ISO C++ Library.  This library is free
// software; you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the
// Free Software Foundation; either version 3, or (at your option)
// any later version.

// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License along
// with this library; see the file COPYING3.  If not see
// <http://www.gnu.org/licenses/>.

#include <vector>
#include "rtpi/mutex.hpp"
#include "rtpi/condition_variable.hpp"
#include <thread>

// PR libstdc++/54185

rtpi::condition_variable* cond = nullptr;
rtpi::mutex mx;
int started = 0;
int constexpr NUM_THREADS = 10;

void do_thread_a()
{
  std::unique_lock<rtpi::mutex> lock(mx);
  if(++started >= NUM_THREADS)
  {
    cond->notify_all(lock);
    delete cond;
    cond = nullptr;
  }
  else
    cond->wait(lock);
}

int main(){
  std::vector<std::thread> vec;
  for(int j = 0; j < 1000; ++j)
  {
    started = 0;
    cond = new rtpi::condition_variable;
    for (int i = 0; i < NUM_THREADS; ++i)
      vec.emplace_back(&do_thread_a);
    for (int i = 0; i < NUM_THREADS; ++i)
      vec[i].join();
    vec.clear();
  }
}

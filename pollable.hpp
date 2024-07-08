#pragma once

#include <vector>

struct Pollable {
  virtual ~Pollable() = 0;
  /**
   * @brief invoking poll lets this module check & respond to any events managed by it.
   *  implementations of this function should never 'block' the calling thread.
   * @return wether the session should exit.
   */
  virtual bool poll() = 0;

  static void poll_loop(const std::vector<Pollable*>&, double sleep_interval_secs = 1.0/25);
};
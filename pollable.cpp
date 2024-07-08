#include "pollable.hpp"

#include <Windows.h>

static constexpr size_t MILISECS_IN_SEC = 1000;

Pollable::~Pollable(){}

void Pollable::poll_loop(const std::vector<Pollable*>& modules, double sleep_interval_secs) {
  while(true) {
    for(Pollable* mod : modules) {
      mod->poll();
      Sleep(static_cast<size_t>(sleep_interval_secs*MILISECS_IN_SEC));
    }
  }
}
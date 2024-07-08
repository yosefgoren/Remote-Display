#pragma once

#include "frames.hpp"
#include "pollable.hpp"

#include <functional>

struct FrameServer : public Pollable{
  FrameServer(std::function<void(frame&)> frame_packet_handler, unsigned portno = 8080);
  virtual bool poll();
};
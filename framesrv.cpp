#include "framesrv.hpp"

FrameServer::FrameServer(std::function<void(frame&)> frame_packet_handler, unsigned portno) {
  
}

bool FrameServer::poll() {
  return false;  
}

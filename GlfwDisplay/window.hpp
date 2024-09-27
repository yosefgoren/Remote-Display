#pragma once

#include "pollable.hpp"
#include "frames.hpp"

#include <functional>
#include <string>
#include <map>

struct FrameWindow : public Pollable{
  FrameWindow(std::map<std::string, std::function<void(FrameWindow*)>> buttons);
  ~FrameWindow();

  /**
   * @brief check and handle any GUI events occured (such as button presses)
   */
  virtual bool poll();

  /**
   * @brief pop the next frame from the frame buffer and show it on the screen
   */
  void pop_frame();
  
  /**
   * @brief request a new frame and return a pointer to it.
   * @return NULL if frame buffer is full, or a pointer to a frame that
   *  can be written by the caller to decide the values of the newly pushed frame.
   *  value will not actually be shown untill this frame is acke'd
   */ 
  frame_t* req_frame();

  void ack_frame();
private:
  void* opaque;
};
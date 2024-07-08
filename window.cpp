#define GLAD_GL_IMPLEMENTATION
#include <glad/gl.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "window.hpp"
#include "frames.hpp"
#include "pollable.hpp"
#include "ring_buf.hpp"

#define OPAQUE_DATA_TYPE WindowData
#include "opaque.hpp"

#include <Windows.h>
#include <linmath.h>
#include <stdexcept>
#include <array>

// Aux functions ===========================

static void drawSquare(float top, float bot, float left, float right, color_t color) {
    glColor3f(color.red, color.green, color.blue);
    glBegin(GL_QUADS);
    glVertex2f(bot, left);
    glVertex2f(top, left);
    glVertex2f(top, right);
    glVertex2f(bot, right);
    glEnd();
}

static void drawCell(int row, int col, color_t color) {
    static const float square_height = (2.0f / HEIGHT_GUEST);
    static const float square_width = (2.0f / WIDTH_GUEST);
    float top = -1.f + (row + 1) * square_height;
    float bot = -1.f + row * square_height;
    float left = -1.f + col * square_width;
    float right = -1.f + (col + 1) * square_width;
    drawSquare(top, bot, left, right, color);
}

static void drawFrame(GLFWwindow* window, const frame_t* frame) {
    glClear(GL_COLOR_BUFFER_BIT);
    for (int row = 0; row < HEIGHT_GUEST; row++) {
        for (int col = 0; col < WIDTH_GUEST; col++) {
            drawCell(row, col, frame->values[row][col]);
        }
    }

    glfwSwapBuffers(window);
}

// Interface Impl ===========================

struct WindowData {
  static constexpr size_t FRAME_BUF_SIZE = 1024;

  WindowData() :window(nullptr), buf(FRAME_BUF_SIZE), num_awaiting_ack(0){}

  GLFWwindow* window;
  RingBuf<frame_t> buf;
  long num_awaiting_ack;
};

FrameWindow::FrameWindow(std::map<std::string, std::function<void(FrameWindow*)>> buttons)
    : opaque(new WindowData()) {
  ENT_METHOD
  if (!glfwInit()) {
      throw std::runtime_error("failed to initialize glfw");
  }

  self.window = glfwCreateWindow(WIDTH_HOST, HEIGHT_HOST, "Grid of Squares", NULL, NULL);
  if (!self.window) {
    glfwTerminate();
    throw std::runtime_error("failed to create glfw window");
  }

  //TODO: check errors for these?
  glfwMakeContextCurrent(self.window);
  gladLoadGL(glfwGetProcAddress);
  // glfwSwapInterval(1);
}

FrameWindow::~FrameWindow() {
  ENT_DTOR
  if(self.window != nullptr) {
    glfwDestroyWindow(self.window);
  }
  glfwTerminate();

  delete (WindowData*)(opaque);
}

bool FrameWindow::poll() {
  ENT_METHOD
  glfwPollEvents();
  
  return glfwWindowShouldClose(self.window);
}

void FrameWindow::pop_frame() {
  ENT_METHOD
  
  if(self.num_awaiting_ack > self.buf.size()) {
    throw std::runtime_error("internal error: there are more non-acked frames then frames in buffer!");
  }

  if(self.num_awaiting_ack < 0) {
    throw std::runtime_error("internal error: number of frames awaiting ack is negative!");
  }

  if(self.num_awaiting_ack == self.buf.size()) {
    throw std::runtime_error("requested to show frame, but there are no acked frames");
  }

  frame_t* frame = self.buf.pop_elem();
  if(frame == nullptr) {
    throw std::runtime_error("requested to show next frame, but frame buffer is empty.");
  }
  drawFrame(self.window, frame);
}

frame_t* FrameWindow::req_frame() {
  ENT_METHOD
  frame_t* frame = self.buf.push_elem();
  if(frame != nullptr) {
    ++self.num_awaiting_ack;
  }
  return frame;
}

void FrameWindow::ack_frame() {
  ENT_METHOD
  if(self.num_awaiting_ack <= 0) {
    throw std::runtime_error("requested to ack, but there are no unacknoladged frames");
  }
  --self.num_awaiting_ack;
}

#undef OPAQUE_DATA_TYPE
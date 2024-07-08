/**
 * This application receives UDP packets from the web, puts them as frames in a buffer, and shows them on the screen.
*/

#include "window.hpp"
#include "framesrv.hpp"

#include <vector>

#include <Windows.h>

static float randFloat() {
    return (float)rand() / RAND_MAX;
}

static color_t randColor() {
    color_t res = {.red = randFloat(), .green = randFloat(), .blue = randFloat()};
    return res;
}

static void putRandFrame(frame_t* frame) {
    for (int row = 0; row < HEIGHT_GUEST; row++) {
        for (int col = 0; col < WIDTH_GUEST; col++) {
            frame->values[row][col] = randColor();
        }
    }
}

void random_noise() {
    // TODO: debug - currently this just shows a white screen :/
    //  can comapre with older original version (previous commit) to find issue
    FrameWindow win{{}};
    while(true) {
        frame_t* frame = win.req_frame();
        if(frame == nullptr) {
            printf("frame request failed!\n");
            return;
        }
        putRandFrame(frame);
        win.ack_frame();

        if(win.poll()){
            break;
        }

        Sleep(100);
    }
}

void webvga() {
    FrameWindow win{{}};
    FrameServer srv{[&win](frame& new_frame){
        frame_t* put_frame = win.req_frame();
        if(put_frame == nullptr) {
            // The buffer is full... Drop this frame.
        }
        memcpy(put_frame, &new_frame, sizeof(*put_frame));
        win.ack_frame();
    }};

    std::vector<Pollable*> modules = {&win, &srv};

    Pollable::poll_loop(modules);
}

int main(void) {
    // webvga();
    random_noise();

    return 0;
}

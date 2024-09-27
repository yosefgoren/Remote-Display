#include <stdio.h>
#include "framesrv.hpp"

int main() {
    printf("frame server starting\n");
    auto srv = FrameServer([](frame& f){
        color_t c = f.values[0][0];
        printf("got frame: %02x %02x %02x\n", c.red, c.green, c.blue);
    });
    std::vector<Pollable*> v = {&srv};
    printf("frame server starting loop\n");
    Pollable::poll_loop(v);
    printf("frame server exisitng\n");
}
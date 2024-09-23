#pragma once

#define CELL_PIXELS 2
#define WIDTH_HOST (WIDTH_GUEST * CELL_PIXELS)
#define HEIGHT_HOST (HEIGHT_GUEST * CELL_PIXELS)
#define WIDTH_GUEST 800
#define HEIGHT_GUEST 600

typedef struct color {
    float red;
    float green;
    float blue;
} color_t;

typedef struct frame {
    color_t values[HEIGHT_GUEST][WIDTH_GUEST];
} frame_t;
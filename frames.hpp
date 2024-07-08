#pragma once

#define CELL_PIXELS 8
#define WIDTH_HOST 3840
#define HEIGHT_HOST 2160
#define WIDTH_GUEST (WIDTH_HOST / CELL_PIXELS)
#define HEIGHT_GUEST (HEIGHT_HOST / CELL_PIXELS)

typedef struct color {
    float red;
    float green;
    float blue;
} color_t;

typedef struct frame {
    color_t values[HEIGHT_GUEST][WIDTH_GUEST];
} frame_t;
#pragma once

#include <stdint.h>

#define CELL_PIXELS 32
#define WIDTH_HOST (WIDTH_GUEST * CELL_PIXELS)
#define HEIGHT_HOST (HEIGHT_GUEST * CELL_PIXELS)
#define WIDTH_GUEST 24
#define HEIGHT_GUEST 20
// #define WIDTH_GUEST 800
// #define HEIGHT_GUEST 600

typedef struct color {
	uint8_t red;
	uint8_t green;
	uint8_t blue;
} color_t;

typedef struct frame {
	color_t values[HEIGHT_GUEST][WIDTH_GUEST];
} frame_t;
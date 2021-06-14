#pragma once

#include <stdint.h>

enum pixel_format { RGB, BGR };

struct frame_buffer_config {
    uint8_t *buf;
    uint32_t d;
    uint32_t h;
    uint32_t v;
    enum pixel_format f;
};

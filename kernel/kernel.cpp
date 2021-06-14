#include <cstdint>
#include <algorithm>
#include <cmath>

#include "frame_buffer_config.hpp"
using namespace std;

struct color {
    uint8_t r, g, b;
};

int write_pixel(const frame_buffer_config &conf, int x, int y, const color &c) {
    uint8_t *p {&conf.buf[4 * (conf.d * y + x)]};
    switch (conf.f) {
    case RGB:
        p[0] = c.r;
        p[1] = c.g;
        p[2] = c.b;
        break;
    case BGR:
        p[0] = c.b;
        p[1] = c.g;
        p[2] = c.r;
        break;
    default:
        return 1;
    }
    return 0;
}

extern "C" void kernel_main (const frame_buffer_config &conf) {
    for (int x {}; x < conf.h; ++x)
        for (int y {}; y < conf.v; ++y)
            write_pixel(conf, x, y, {255, 255, 255});
    for (int x {}; x < 100; x++)
        for (int y {}; y < 200; y++)
            write_pixel(conf, x + 10, y + 20, {255, 100, 100});
    for (;;)
        __asm__("hlt");
}

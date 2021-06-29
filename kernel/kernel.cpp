#include <cstdlib>

#include "range.hpp"
#include "graphics.hpp"
#include "font.hpp"
using namespace std;

void* operator new([[maybe_unused]] size_t s, void *buf) { return buf; }
void operator delete([[maybe_unused]] void * o) noexcept {}

extern "C" void kernel_main (const frame_buffer_config &conf) {
    char writer_buf[sizeof(pixel_writer)];
    pixel_writer *writer = conf.f == RGB ?
        (pixel_writer *) new(writer_buf) rgb_writer(conf) :
        (pixel_writer *) new(writer_buf) bgr_writer(conf);

    for (int x: range(conf.h))
        for (int y: range(conf.v))
            writer->write(x, y, {255, 255, 255});
    for ([[maybe_unused]] int t: range(10))
        for (int x {}, ex = rand() % conf.h / 2, ey = rand() % conf.v / 2; x < ex; x++)
            for (int y: range(ey))
                writer->write(x + ex, y + ey, {(uint8_t)(ex % 256), (uint8_t)(ey % 256), 0});

    char s[] = "Hello, boxnos-m World !?";
    for (int i {}; s[i]; ++i) {
        write_ascii(*writer, 201 + i * 6, 301, s[i], {0x00, 0x00, 0x00});
        write_ascii(*writer, 200 + i * 6, 300, s[i], {0xFF, 0xFF, 0xFF});
    }

    for (;;)
        __asm__("hlt");
}

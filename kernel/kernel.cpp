#include <cstdlib>
#include <cstdio>

#include "range.hpp"
#include "graphics.hpp"
#include "font.hpp"
#include "console.hpp"
using namespace std;

char konsole_buf[sizeof(console)];
console *konsole;

void* operator new([[maybe_unused]] size_t s, void *buf) { return buf; }
void operator delete([[maybe_unused]] void * o) noexcept {}

int printk(const char* s, ...) {
    int r;
    va_list ap;
    static char b[1024];

    va_start(ap, s);
    r = vsprintf(b, s, ap);
    va_end(ap);

    konsole->put_string(b);

    return r;
}

const int mouse_width {12}, mouse_height {19};
const char mouse[mouse_height][mouse_width + 1] {
  "@           ",
  "@@          ",
  "@.@         ",
  "@..@        ",
  "@...@       ",
  "@....@      ",
  "@.....@     ",
  "@......@    ",
  "@.......@   ",
  "@........@  ",
  "@.........@ ",
  "@..........@",
  "@......@@@@@",
  "@...@..@    ",
  "@..@ @..@   ",
  "@.@  @..@   ",
  "@@    @..@  ",
  "      @..@  ",
  "       @@   ",
};


extern "C" void kernel_main (const frame_buffer_config &conf) {
    char writer_buf[sizeof(pixel_writer)];
    pixel_writer *writer = conf.f == RGB ?
        (pixel_writer *) new(writer_buf) rgb_writer(conf) :
        (pixel_writer *) new(writer_buf) bgr_writer(conf);

    writer->rect(0, 0, conf.h, conf.v, {0xFF, 0xFF, 0xFF});
    for ([[maybe_unused]] int t: range(10))
        for (int x {}, ex = rand() % conf.h / 2, ey = rand() % conf.v / 2; x < ex; x++)
            for (int y: range(ey))
                writer->write(x + ex, y + ey, {(uint8_t)(ex % 256), (uint8_t)(ey % 256), 0});

    konsole = new(konsole_buf) console(*writer, {0xFF, 0xFF, 0xFF}, {0x33, 0x33, 0x33});

    char buf[1024];
    write_string(*writer, 200, 300, "Hello, boxnos-m World !?", {0xFF, 0xFF, 0xFF});
    sprintf(buf, "123 * 456 = %d", 123 * 456);
    write_string(*writer, 200, 313, buf, {0xFF, 0xFF, 0xFF});

    for (int i: range(1, 20))
        printk("long long long long long line : %d\n", i);
    printk("Wellcome to the boxnos world!!\n");

    for (int my: range(mouse_height))
        for (int mx: range(mouse_width))
            if (mouse[my][mx] != ' ')
                writer->write(200 + mx, 100 + my,
                              mouse[my][mx] == '.' ? color{0xFF, 0xFF, 0xFF} : color{0x00, 0x00, 0x00});

    for (;;)
        __asm__("hlt");
}

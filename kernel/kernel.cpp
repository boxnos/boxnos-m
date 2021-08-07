#include <cstdlib>
#include <cstdio>

#include "range.hpp"
#include "graphics.hpp"
#include "font.hpp"
#include "console.hpp"
#include "pci.hpp"
using namespace std;

//void* operator new([[maybe_unused]] size_t s, void *buf) { return buf; }
void operator delete([[maybe_unused]] void * o) noexcept {}

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


    writer->fill_rect(0, 0, conf.h, conf.v, {0xFF, 0xFF, 0xFF});
    for ([[maybe_unused]] int t: range(15)) {
        int ex = rand() % conf.h / 2, ey = rand() % conf.v / 2;
        writer->fill_rect({ex, ey}, {ex, ey}, {uint8_t(ex % 256), uint8_t(ey % 256), 0});
    }

    for ([[maybe_unused]] int t: range(15)) {
        int ex = rand() % conf.h / 2, ey = rand() % conf.v / 2;
        writer->draw_rect({ex, ey}, {ex, ey}, {uint8_t(ex % 256), uint8_t(ey % 256), 0});
    }

    konsole = new(konsole_buf) console(*writer, {0xFF, 0xFF, 0xFF}, {0x33, 0x33, 0x33});

    //char buf[1024];
    //write_string(*writer, 200, 300, "Hello, boxnos-m World !?", {0xFF, 0xFF, 0xFF});
    //sprintf(buf, "123 * 456 = %d", 123 * 456);
    //write_string(*writer, 200, 313, buf, {0xFF, 0xFF, 0xFF});


    printk("Wellcome to the boxnos world!!\n");
    //for (int i: range(1, 10))
    //    printk("long long long long long line : %d\n", i);

    //V2 a {1, 2}, b {3, 4};
    //printk("a(%d, %d) += b(%d, %d)\n", a.x, a.y, b.x, b.y);
    //a += b;
    //printk("a(%d, %d)\n", a.x, a.y);

    // start menu
    writer->fill_rect(0, conf.v - 30, conf.h, 30, {0x33, 0x33, 0x33});
    writer->fill_rect(2, conf.v - 28, 80, 26, {0x44, 0x44, 0x44});
    writer->draw_rect({2, int(conf.v - 28)}, {80, 26}, {0x99, 0x99, 0x99});
    write_string(*writer, 10, conf.v - 21, "   START   ", {0xFF, 0xFF, 0xFF});

    printk("status : %s\n", pci::scan_all_bus().name());

    takl("scan all bus");

    for (int i: range(pci::num_device)) {
        auto &dev = pci::devices[i];
        printk("%d %d %d vend : %04x class : %08x, head %02x\n", dev.bus, dev.device, dev.function,
               pci::read_vender_id(dev.bus, dev.device, dev.function),
               pci::read_class_code(dev.bus, dev.device, dev.function), dev.header_type);
    }

    for (int my: range(mouse_height))
        for (int mx: range(mouse_width))
            if (mouse[my][mx] != ' ')
                writer->write(300 + mx, 100 + my,
                              mouse[my][mx] == '.' ? color{0xFF, 0xFF, 0xFF} : color{0x00, 0x00, 0x00});


    for (;;)
        __asm__("hlt");
}

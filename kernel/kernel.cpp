#include <cstddef>
#include <cstdint>
#include <cstdlib>

#include "frame_buffer_config.hpp"
using namespace std;

struct color {
    uint8_t r, g, b;
};

class pixel_writer {
    private:
        const frame_buffer_config conf;
    public:
        pixel_writer(const frame_buffer_config &c) : conf {c} {}
        virtual ~pixel_writer() = default;
        virtual void write(int x, int y, const color &c) = 0;
    protected:
        uint8_t* at(int x, int y) {
            return &conf.buf[4 * (conf.d * y + x)];
        }
};

class rgb_writer : public pixel_writer {
    public:
        using pixel_writer::pixel_writer;
        virtual void write(int x, int y, const color &c) override {
            auto p = at(x, y);
            p[0] = c.r;
            p[1] = c.g;
            p[2] = c.b;
        }
};

class bgr_writer : public pixel_writer {
    public:
        using pixel_writer::pixel_writer;
        virtual void write(int x, int y, const color &c) override {
            auto p = at(x, y);
            p[0] = c.b;
            p[1] = c.g;
            p[2] = c.r;
        }
};

void* operator new(size_t s, void *buf) { return buf; }
void operator delete(void *o) {}

extern "C" void kernel_main (const frame_buffer_config &conf) {
    char writer_buf[sizeof(pixel_writer)];
    pixel_writer *writer = conf.f == RGB ?
        (pixel_writer *) new(writer_buf) rgb_writer(conf) :
        (pixel_writer *) new(writer_buf) bgr_writer(conf);

    for (int x {}; x < conf.h; ++x)
        for (int y {}; y < conf.v; ++y)
            writer->write(x, y, {255, 255, 255});
    for (int t {}; t < 10; t++)
        for (int x {}, ex = rand() % conf.h / 2, ey = rand() % conf.v / 2; x < ex; x++)
            for (int y {}; y < ey; y++)
                writer->write(x + ex, y + ey, {(uint8_t)(ex % 256), (uint8_t)(ey % 256), 0});
    for (;;)
        __asm__("hlt");
}

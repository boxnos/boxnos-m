#include <cstddef>
#include <cstdint>
#include <cstdlib>

#include "frame_buffer_config.hpp"
using namespace std;

template <typename T=int> struct range{
    T e,b=0,s=1;range(T b,T e,T s):e(e),b(b),s(s){} range(T b,T e): e(e), b(b){} range(T e):e(e){}
    struct it{T v,s; it(T v,T s):v(v),s(s){} operator T()const{return v;} inline operator T&(){return v;}T operator*()const{return v;}
        inline it& operator++(){v+=s;return *this;}}; it begin(){return {b,s};} it end(){return {e,s};}};

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

const uint8_t font[16] {
    0b00000000,
    0b01100000,
    0b10010000,
    0b10010000,
    0b10010000,
    0b11110000,
    0b10010000,
    0b10010000,
    0b10010000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000
};

void write_ascii(pixel_writer &writer, int x, int y, char c, const color r) {
    for (int i : range(10))
        for (int j : range(5))
            if ((font[i] << j) & 0x80u)
                writer.write(x + j, y + i, r);
}

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

    for (int i: range(10))
        write_ascii(*writer, 100 + i * 5, 100, 'A', {0, 0, 0});

    for (;;)
        __asm__("hlt");
}

#pragma once
#include "frame_buffer_config.hpp"

struct color {
    uint8_t r, g, b;
};

class pixel_writer {
    private:
        const frame_buffer_config conf;
    public:
        pixel_writer(const frame_buffer_config &c) : conf {c} {}
        void rect(int, int, int, int, const color &);
        virtual ~pixel_writer() = default;
        virtual void write(int, int, const color &) = 0;
    protected:
        uint8_t* at(int x, int y) {
            return &conf.buf[4 * (conf.d * y + x)];
        }
};

class rgb_writer : public pixel_writer {
    public:
        using pixel_writer::pixel_writer;
        virtual void write(int, int, const color &) override;
};

class bgr_writer : public pixel_writer {
    public:
        using pixel_writer::pixel_writer;
        virtual void write(int, int, const color &) override;
};

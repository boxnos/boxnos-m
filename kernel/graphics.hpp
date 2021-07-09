#pragma once
#include "frame_buffer_config.hpp"

struct color {
    uint8_t r, g, b;
};

struct V2 {
    int x, y;
    V2 &operator +=(const V2 &a) {
        x += a.x;
        y += a.y;
        return *this;
    }
    V2 operator +(const V2 &a) {
        return {x + a.x, y + a.y};
    }
};

class pixel_writer {
    private:
        const frame_buffer_config conf_;
    public:
        pixel_writer(const frame_buffer_config &c) : conf_ {c} {}
        virtual ~pixel_writer() = default;
        virtual void write(int, int, const color &) = 0;
        void rect(int, int, int, int, const color &);
        void rect(const V2 &, const V2 &, const color &);
    protected:
        uint8_t* at(int x, int y) {
            return &conf_.buf[4 * (conf_.d * y + x)];
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

#pragma once

#include "graphics.hpp"

class console {
    public:
        static const int rows {30}, cols {50};

        console(pixel_writer &, const color &, const color &);
        void put_string(const char *);
    private:
        pixel_writer& writer_;
        const color fg_, bg_;
        int row_, col_;
        char buf_[rows][cols + 1];

        void new_line();
};

extern char konsole_buf[];
extern console *konsole;

int printk(const char *s, ...);

#define TAK(t, s) inline t tak(t v) {printk(s, v); return v;}
TAK(const char *, "%s") TAK(uint8_t, "0x%x")
template <typename T> inline T takl(T v) {tak(v); printk("\n"); return v;}
template <typename T> inline T taks(T v) {tak(v); printk(" "); return v;}

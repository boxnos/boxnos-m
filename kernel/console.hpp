#pragma once

#include "graphics.hpp"

class console {
    public:
        static const int rows {10}, cols {20};

        console(pixel_writer &, const color &, const color &);
        void put_string(const char *);
    private:
        pixel_writer& writer_;
        const color fg_, bg_;
        int row_, col_;
        char buf_[rows][cols + 1];

        void new_line();
};

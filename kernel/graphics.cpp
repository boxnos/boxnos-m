#include "graphics.hpp"

void pixel_writer::rect(int x, int y, int w, int h, const color &c) {
    for (int i {}; i < h; i++)
        for (int j {}; j < w; j++)
            write(x + j, y + i, c);
}

void rgb_writer::write(int x, int y, const color &c) {
    auto p = at(x, y);
    p[0] = c.r;
    p[1] = c.g;
    p[2] = c.b;
}

void bgr_writer::write(int x, int y, const color &c) {
    auto p = at(x, y);
    p[0] = c.b;
    p[1] = c.g;
    p[2] = c.r;
}

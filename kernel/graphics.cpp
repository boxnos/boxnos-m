#include "graphics.hpp"
#include <algorithm>
using namespace std;

void pixel_writer::rect(int x, int y, int w, int h, const color &c) {
    w = min(int(conf_.h - x), w);
    h = min(int(conf_.v - y), h);
    for (int i {}; i < h; i++)
        for (int j {}; j < w; j++)
            write(x + j, y + i, c);
}

void pixel_writer::rect(const V2 &s, const V2 &r, const color &c) {
    rect(s.x, s.y, r.x, r.y, c);
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

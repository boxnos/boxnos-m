#include "graphics.hpp"
#include "range.hpp"
#include <algorithm>
using namespace std;

void pixel_writer::fill_rect(int x, int y, int w, int h, const color &c) {
    for (int i: range(min(int(conf_.v - y), h)))
        for (int j: range(min(int(conf_.h - x), w)))
            write(x + j, y + i, c);
}

void pixel_writer::fill_rect(const V2 &s, const V2 &r, const color &c) {
    fill_rect(s.x, s.y, r.x, r.y, c);
}

void pixel_writer::draw_rect(const V2 &s, const V2 &r, const color &c) {
    for (int i: range(r.x)) {
        write(s.x + i, s.y, c);
        write(s.x + i, s.y + r.y - 1, c);
    }
    for (int i: range(1, r.y - 1)) {
        write(s.x, s.y + i, c);
        write(s.x + r.x - 1, s.y + i, c);
    }
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

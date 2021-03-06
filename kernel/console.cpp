#include <cstring>
#include <stdio.h>
#include "console.hpp"
#include "font.hpp"
#include "range.hpp"

char konsole_buf[sizeof(console)];
console *konsole;

console::console (pixel_writer &writer, const color &fg, const color &bg) : writer_{writer}, fg_{fg}, bg_{bg} {
    writer_.fill_rect(0, 0, 6 * cols, 13 * rows, bg_);
}

void console::new_line() {
    col_ = 0;
    if (row_ < rows - 1) {
        ++row_;
    } else {
        writer_.fill_rect(0, 0, 6 * cols, 13 * rows, bg_);
        for (int r: range(rows - 1)) {
            memcpy(buf_[r], buf_[r + 1], cols);
            write_string(writer_, 0, 13 * r, buf_[r], fg_);
        }
        memset(buf_[rows - 1], 0, cols);
    }
}

void console::put_string (const char *s) {
    for (; *s; ++s)
        if (*s == '\n')
            new_line();
        else {
            if (col_ >= cols)
                new_line();
            buf_[row_][col_] = *s;
            write_ascii(writer_, 6 * col_++, 13 * row_, *s, fg_);
        }
}


int printk(const char *s, ...) {
    int r;
    va_list ap;
    char b[1024];

    va_start(ap, s);
    r = vsprintf(b, s, ap);
    va_end(ap);

    konsole->put_string(b);

    return r;
}

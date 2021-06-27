#include "font.hpp"
#include "range.hpp"

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

void write_ascii(pixel_writer &writer, int x, int y, [[maybe_unused]] char c, const color r) {
    for (int i : range(10))
        for (int j : range(5))
            if ((font[i] << j) & 0x80u)
                writer.write(x + j, y + i, r);
}

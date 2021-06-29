#include "font.hpp"
#include "range.hpp"

extern const uint8_t _binary_mplus_bin_start;
extern const uint8_t _binary_mplus_bin_end;
extern const uint8_t _binary_mplus_bin_size;

const uint8_t * get_font(char c) {
    return &_binary_mplus_bin_start + 13 * c;
}

void write_ascii(pixel_writer &writer, int x, int y, [[maybe_unused]] char c, const color r) {
    auto addr {get_font(c)};
    for (int i : range(13))
        for (int j : range(6))
            if ((addr[i] << j) & 0x80u)
                writer.write(x + j, y + i, r);
}

#pragma once

#include <stdint.h>

extern "C" {
    void io_out32(uint16_t, uint32_t);
    uint32_t io_in32(uint16_t);
}

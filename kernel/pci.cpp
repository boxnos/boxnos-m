#include "pci.hpp"

uint32_t make_address(uint8_t bus, uint8_t device, uint8_t function, uint8_t reg_addr) {
    return (1 << 31 | bus << 16 | device << 11 | function << 8 | (reg_addr & 0xfcu));
}


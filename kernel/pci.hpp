#pragma once
#include <cstdint>
#include <array>

namespace pci {
    uint32_t make_address(uint8_t, uint8_t, uint8_t, uint8_t);
    uint16_t read_vender_id(uint8_t, uint8_t, uint8_t);
    uint32_t read_class_code(uint8_t, uint8_t, uint8_t);
    void scan_all_bus();

    struct device {
        uint8_t bus, device, function, header_type;
    };

    inline std::array<device, 32> devices;
    inline int num_device;
}

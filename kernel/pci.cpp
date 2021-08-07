#include "console.hpp"
#include "pci.hpp"
#include "asmfunc.h"

namespace pci {
    uint32_t make_address(uint8_t bus, uint8_t device, uint8_t function, uint8_t reg_addr) {
        return (1 << 31 | bus << 16 | device << 11 | function << 8 | (reg_addr & 0xfcu));
    }

    void write_address(uint32_t addr) {
        io_out32(0x0cf8, addr);
    }

    uint32_t read_data() {
        return io_in32(0x0cfc);
    }

    uint16_t read_vender_id(uint8_t bus, uint8_t device, uint8_t function) {
        write_address(make_address(bus, device, function, 0x00));
        return read_data() & 0xffffu;
    }
    uint8_t read_header_type(uint8_t bus, uint8_t device, uint8_t function) {
        write_address(make_address(bus, device, function, 0x0c));
        return (read_data() >> 16) & 0xffu;
    }

    uint32_t read_class_code(uint8_t bus, uint8_t device, uint8_t function) {
        write_address(make_address(bus, device, function, 0x08));
        return read_data();
    }

    bool is_single_function_device(uint8_t header_type) {
        return (header_type & 0x80u) == 0;
    }

    void add_device(uint8_t bus, uint8_t dev, uint8_t function, uint8_t header_type) {
        devices[num_device++] = device {bus, dev, function, header_type};
    }

    void scan_function(uint8_t bus, uint8_t device, uint16_t function) {
        add_device(bus, device, function, read_header_type(bus, device, function));

        // for PCI-PCI bridge not tested
        auto class_code = read_class_code(bus, device, function);
        if (((class_code >> 24) & 0xffu) == 0x06u &&
            ((class_code >> 16) & 0xffu) == 0x04u)
            printk("PCI-PCI detected\n");
    }

    void scan_device(uint8_t bus, uint8_t device) {
        if (is_single_function_device(read_header_type(bus, device, 0)))
            return;
        for (uint8_t f {1}; f < 8 ;++f)
            if (read_vender_id(bus, device, f) != 0xffffu)
                scan_function(bus, device, f);
    }

    void scan_bus(uint8_t bus) {
        for (uint8_t d {}; d < 32; ++d) {
            if (read_vender_id(bus, d, 0) != 0xffffu)
                scan_device(bus, d);
        }
    }

    error scan_all_bus() {
        num_device = 0;
        if (is_single_function_device(read_header_type(0, 0, 0)))
            scan_bus(0);
        for (uint8_t f {1}; f < 8; ++f) {
            if (read_vender_id(0, 0, f) != 0xffffu)
                scan_bus(f);
        }
        printk("io_in32(0x0cf8) : %u\n", io_in32(0x0cf8));
        printk("make_address(1, 4, 0, 0x04) : %x\n", make_address(1, 4, 0, 0x04));
        printk("io_out32(0x0cf8, make_address(1, 4, 0, 0x04)\n");
        io_out32(0x0cf8, make_address(1, 4, 0, 0x04));
        printk("io_in32(0x0cfc) : %x\n", io_in32(0x0cfc));
        return error::success;
    }
}

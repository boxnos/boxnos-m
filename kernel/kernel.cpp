#include <cstdint>
#include <algorithm>
#include <cmath>
using namespace std;

extern "C" void kernel_main (uint64_t frame_buffer_base, uint64_t frame_buffer_size) {
    uint8_t* frame_buffer = reinterpret_cast<uint8_t*>(frame_buffer_base);
    for (uint64_t i {}; i < frame_buffer_size; i += 4) {
        frame_buffer[i] = 0x00; // B
        frame_buffer[i+1] = 256. * sin(i / 256.) + 0xFF; // G
        frame_buffer[i+2] = 256. * sin(i / 256.) + 0xFF; // R
        frame_buffer[i+3] = 0x00;
    }
    for (;;)
        __asm__("hlt");
}

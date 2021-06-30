#pragma once

#include "graphics.hpp"
void write_ascii(pixel_writer &writer, int x, int y, char c, const color r);
void write_string(pixel_writer &writer, int x, int y, const char *s, const color r);

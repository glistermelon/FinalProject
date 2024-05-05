#pragma once

struct Color {
    unsigned char r, g, b;
    Color(unsigned char r, unsigned char g, unsigned char b);
    static Color BLACK, WHITE, RED, GREEN, BLUE, YELLOW, MAGENTA, CYAN;
};
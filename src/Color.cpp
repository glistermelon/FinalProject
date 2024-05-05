#include <Color.h>

Color Color::BLACK { 0, 0, 0 };
Color Color::WHITE { 255, 255, 255 };
Color Color::RED { 255, 0, 0 };
Color Color::GREEN { 0, 255, 0 };
Color Color::BLUE { 0, 0, 255 };
Color Color::YELLOW { 255, 255, 0 };
Color Color::MAGENTA { 255, 0, 255 };
Color Color::CYAN { 0, 255, 255 };

Color::Color(unsigned char r, unsigned char g, unsigned char b) : r(r), g(g), b(b) {}

#include "../include/Point.h"

Point::Point(double x, double y) : x(x), y(y) {}

std::ostream& operator<<(std::ostream& o, const Point& p) {
    return o << "(" << p.x << "," << p.y << ")";
}

bool operator==(const Point& a, const Point& b) {
    return a.x == b.x && a.y == b.y;
}
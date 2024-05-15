#include "../include/Point.h"

#include "../include/Vect2.h"

Point::Point(double x, double y) : x(x), y(y) {}

std::ostream& operator<<(std::ostream& o, const Point& p) {
    return o << "(" << p.x << "," << p.y << ")";
}

bool operator==(const Point& a, const Point& b) {
    return a.x == b.x && a.y == b.y;
}

void Point::revolve(double angle) {
    Vect2 v(x, y);
    v.rotate(angle);
    x = v.x;
    y = v.y;
}

bool Point::epsilon_eq(Point other) const {
    constexpr double epsilon = 0.0001;
    return abs(x - other.x) <= epsilon && abs(y - other.y) <= epsilon;
}

bool operator!=(const Point& a, const Point& b) {
    return !(a == b);
}
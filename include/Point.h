#pragma once

#include <ostream>

struct Point {

    double x, y;

    Point(double x = 0, double y = 0);

    void revolve(double angle);

    bool epsilon_eq(Point other) const;

};

std::ostream& operator<<(std::ostream& o, const Point& p);
bool operator==(const Point& a, const Point& b);
bool operator!=(const Point& a, const Point& b);
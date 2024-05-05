#pragma once

#include <ostream>

struct Point {

    friend std::ostream& operator<<(std::ostream& o, const Point& p);
    friend bool operator==(const Point& a, const Point& b);

    double x, y;

public:

    Point(double x = 0, double y = 0);

};

std::ostream& operator<<(std::ostream& o, const Point& p);
bool operator==(const Point& a, const Point& b);
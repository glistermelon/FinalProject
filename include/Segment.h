#pragma once

#include "Point.h"

class Segment {

    double slope;

    friend bool operator==(const Segment& a, const Segment& b);

public:

    Point p1, p2;

    inline Segment() : slope(0) {}

    inline Segment(Point p1, Point p2) : p1(p1), p2(p2), slope((p1.y - p2.y) / (p1.x - p2.x)) {}

    inline decltype(slope) get_slope() { return slope; }

    bool intersects(Segment other) const;

    double get_y(double x) const; // x relative to p1.x

};

bool operator==(const Segment& a, const Segment& b);
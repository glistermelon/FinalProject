#pragma once

#include "Point.h"

struct Triangle {

    Point a, b, c;

    Triangle(Point a, Point b, Point c);

    Point centroid(); // this is also the triangle's center of mass!

};
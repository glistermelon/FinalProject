#include "Triangle.h"


Triangle::Triangle(Point a, Point b, Point c) : a(a), b(b), c(c) {}

Point Triangle::centroid() { // TODO
    return Point{(a.x+b.x+c.x)/3.0, (a.y+b.y+c.y)/3.0};
}
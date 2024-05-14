#include "Triangle.h"


Triangle::Triangle(Point a, Point b, Point c) : a(a), b(b), c(c) {}

Point Triangle::centroid() {
    return Point{(a.x+b.x+c.x)/3.0, (a.y+b.y+c.y)/3.0};
}

// Didn't have time to do the math inequalities, so this solution is based off this stackoverflow thread:
// https://stackoverflow.com/questions/2049582/how-to-determine-if-a-point-is-in-a-2d-triangle
bool Triangle::contains(Point p) const {
    double sign1 = (p.x - b.x) * (a.y - b.y) - (a.x - b.x) * (p.y - b.y);
    double sign2 = (p.x - c.x) * (b.y - c.y) - (b.x - c.x) * (p.y - c.y);
    double sign3 = (p.x - a.x) * (c.y - a.y) - (c.x - a.x) * (p.y - a.y);
    return !((sign1 < 0 || sign2 < 0 || sign3 < 0) && (sign1 > 0 || sign2 > 0 || sign3 > 0));
}

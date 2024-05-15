#include "../include/Segment.h"

// https://www.geeksforgeeks.org/check-if-two-given-line-segments-intersect/

bool Segment::contains_point(Point p) const {
    return p.x <= std::max(p1.x, p2.x) && p.y <= std::max(p1.y, p2.y)
        && p.x >= std::min(p1.x, p2.x) && p.y >= std::min(p1.y, p2.y);
}

enum ORIENTATION { COLLINEAR, CW, CCW };
ORIENTATION check_orientation(Point a, Point b, Point c) {
    double o = (b.y - a.y) * (c.x - b.x) -
               (b.x - a.x) * (c.y - b.y);
    if (abs(o) < 0.0001) return COLLINEAR;
    return o > 0 ? CW : CCW;
}

bool Segment::intersects(Segment other) const {

    ORIENTATION o[4] = {
            check_orientation(p1, p2, other.p1),
            check_orientation(p1, p2, other.p2),
            check_orientation(other.p1, other.p2, p1),
            check_orientation(other.p1, other.p2, p2)
    };

    return (
            (o[0] != o[1] && o[2] != o[3]) ||
            (o[0] == 0 && this->contains_point(other.p1)) ||
            (o[1] == 0 && Segment(p1, other.p2).contains_point(p2)) ||
            (o[2] == 0 && Segment(other.p1, p1).contains_point(other.p2)) ||
            (o[3] == 0 && Segment(other.p1, p2).contains_point(other.p2))
    );

}

double Segment::get_y(double x) const {
    return slope * (x - p1.x) + p1.y;
}

bool operator==(const Segment& a, const Segment& b) {
    return (a.p1 == b.p1 && a.p2 == b.p2) || (a.p1 == b.p2 && a.p2 == b.p1);
}
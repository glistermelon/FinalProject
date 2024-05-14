#include "../include/Segment.h"

bool Segment::intersects(Segment other) const {

    auto x = other.p1.y - p1.y - other.slope * other.p1.x + slope * p1.x / (slope - other.slope);

    auto domain1_left = p1.x;
    auto domain1_right = p2.x;
    if (domain1_left > domain1_right) std::swap(domain1_left, domain1_right);

    auto domain2_left = other.p1.x;
    auto domain2_right = other.p2.x;
    if (domain2_left > domain2_right) std::swap(domain2_left, domain2_right);

    return x >= domain1_left && x >= domain2_left && x <= domain1_right && x <= domain2_right);

}

double Segment::get_y(double x) const {
    return slope * (x - p1.x) + p1.y;
}

bool operator==(const Segment& a, const Segment& b) {
    return (a.p1 == b.p1 && a.p2 == b.p2) || (a.p1 == b.p2 && a.p2 == b.p1);
}
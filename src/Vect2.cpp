#include "../include/Vect2.h"
#include <cmath>


Vect2::Vect2() {
    value1 = 0;
    value2 = 0;
}

Vect2::Vect2(double v1, double v2) {
    value1 = v1;
    value2 = v2;
}

double Vect2::direction() {
    return atan(value2 / value1);
}

double Vect2::magnitude() {
    return sqrt(pow(value1, 2) + pow(value2, 2));
}

Vect2 Vect2::operator*(double scalar) {
    return Vect2((value1 * scalar), (value2 * scalar));
}

Vect2 Vect2::operator/(double scalar) {
    return Vect2((value1 / scalar), (value2 / scalar));
}

Vect2 Vect2::operator+(Vect2 v) {
    return Vect2((value1 + v.x), (value2 + v.y));
}

Vect2 Vect2::operator-(Vect2 v) {
    return Vect2((value1 - v.x), (value2 - v.y));
}

double Vect2::dot_product(Vect2 v1, Vect2 v2) {
    return ((v1.x * v2.x) + (v1.y * v2.y));
}

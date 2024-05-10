#include "../include/Vect2.h"
#define _USE_MATH_DEFINES
#include <cmath>
#include <assert.h>

Vect2::Vect2() {
    value1 = 0;
    value2 = 0;
}

Vect2::Vect2(double v1, double v2) {
    value1 = v1;
    value2 = v2;
}

double Vect2::direction() {
    if (x == 0) {
        if (y > 0) {
            return (M_PI / 2); 
        }
        else if (y < 0) {
            return (3 * (M_PI / 2));
        }
        else {
            //You can't call direction() on the zero vector, silly!
            assert(false);
        }
    }
    else {
        double angleRadians = atan(value2 / value1);
        if (x < 0) {
            angleRadians += M_PI;
        }
        else if (y < 0) {
            angleRadians += (M_PI * 2);
        }
        return angleRadians;
    }
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
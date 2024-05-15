#include "../include/Vect2.h"
#define _USE_MATH_DEFINES
#include <cmath>
#include <assert.h>

Vect2::Vect2() {
    x = 0;
    y = 0;
}

Vect2::Vect2(double v1, double v2) {
    x = v1;
    y = v2;
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
        double angleRadians = atan(y / x);
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
    return sqrt(pow(x, 2) + pow(y, 2));
}

Vect2 Vect2::operator*(double scalar) {
    return Vect2((x * scalar), (y * scalar));
}

Vect2 Vect2::operator/(double scalar) {
    return Vect2((x / scalar), (y / scalar));
}

Vect2 Vect2::operator+(Vect2 v) {
    return Vect2((x + v.x), (y + v.y));
}

Vect2 Vect2::operator-(Vect2 v) {
    return Vect2((x - v.x), (y - v.y));
}

Vect2 Vect2::operator-() {
    auto v = *this;
    v.flip();
    return v;
}

double Vect2::dot_product(Vect2 v1, Vect2 v2) {
    return ((v1.x * v2.x) + (v1.y * v2.y));
}

void Vect2::flip() {
    x = -x;
    y = -y;
}

void Vect2::set_magnitude(double m) {
    double f = m / magnitude();
    x *= f;
    y *= f;
}

void Vect2::rotate(double angle) {
    double m = magnitude();
    angle += direction();
    x = m * cos(angle);
    y = m * sin(angle);
}

Vect3 Vect2::to_3d() const {
    return Vect3(x, y, 0);
}

Vect3::Vect3() : x(0), y(0), z(0) {}

Vect3::Vect3(double v1, double v2, double v3) : x(v1), y(v2), z(v3) {}

double Vect3::magnitude() {
    return sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));
}

Vect3 Vect3::operator*(double scalar) {
    return Vect3((x * scalar), (y * scalar), (z * scalar));
}

Vect3 Vect3::operator/(double scalar) {
    return Vect3((x / scalar), (y / scalar), (z / scalar));
}

Vect3 Vect3::operator+(Vect3 v) {
    return Vect3((x + v.x), (y + v.y), (z + v.z));
}

Vect3 Vect3::operator-(Vect3 v) {
    return Vect3((x - v.x), (y - v.y), (z - v.z));
}

Vect3 Vect3::operator-() {
    auto v = *this;
    v.flip();
    return v;
}

void Vect3::flip() {
    x = -x;
    y = -y;
    z = -z;
}

void Vect3::set_magnitude(double m) {
    double f = m / magnitude();
    x *= f;
    y *= f;
    z *= f;
}

Vect3 Vect3::cross_product(Vect3 v1, Vect3 v2) {
    return Vect3(v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z, v1.x * v2.y - v1.y * v2.x);
}

Vect2 Vect3::to_2d() const {
    return Vect2(x, y);
}

Vect3 operator*(double n, Vect3 v) {
    return v * n;
}
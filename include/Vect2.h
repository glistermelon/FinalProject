#pragma once

struct Vect2 {

    double value1, value2;

    // for ease when the vector refers to p1 spatial coordinate
    double x = value1;
    double y = value2;

    Vect2(); // set both values to zero
    Vect2(double v1, double v2);

    // about the unit circle, as standard; returns radians
    double direction();

    // length
    double magnitude();

    Vect2 operator*(double scalar);
    Vect2 operator/(double scalar);

    Vect2 operator+(Vect2 v);
    Vect2 operator-(Vect2 v);
    Vect2 operator-();

    static double dot_product(Vect2, Vect2);

    void flip();

    void set_magnitude(double);

    void rotate(double); // rotates counterclockwise

};
#pragma once

struct Vect3;

struct Vect2 {

    double x;
    double y;

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

    Vect3 to_3d() const;

};

struct Vect3 {

    double x;
    double y;
    double z;

    Vect3(); // set all values to zero
    Vect3(double v1, double v2, double v3);

    // length
    double magnitude();

    Vect3 operator*(double scalar);
    Vect3 operator/(double scalar);

    Vect3 operator+(Vect3 v);
    Vect3 operator-(Vect3 v);
    Vect3 operator-();

    void flip();

    void set_magnitude(double);

    static Vect3 cross_product(Vect3, Vect3);

    Vect2 to_2d() const;

};

Vect3 operator*(double, Vect3);
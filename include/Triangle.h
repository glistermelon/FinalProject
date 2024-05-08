#pragma once

#include "Point.h"

struct Triangle {

    Point a, b, c;

    Point centroid(); // this is also the triangle's center of mass!

};
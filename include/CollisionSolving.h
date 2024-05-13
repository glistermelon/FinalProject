#pragma once

#include "Point.h"
#include "Vect2.h"
#include "Block.h"

// sequential impulses
// https://box2d.org/files/ErinCatto_UnderstandingConstraints_GDC2014.pdf
// https://box2d.org/files/ErinCatto_ModelingAndSolvingConstraints_GDC2009.pdf
// http://www.richardtonge.com/presentations/Tonge-2012-GDC-solvingRigidBodyContacts.pdf

class Collision {

    Block* block1, *block2;
    Point point1, point2;
    Vect2 normal;

    double p1_total_impulse = 0, p2_total_impulse = 0; // cumulative impulse

    double calc_impulse(Point); // argument should be either point1 or point2

public:

    Collision(Block* b1, Block* b2, Point p1, Point p2, Vect2 normal);

    void solve();

};

class CollisionGroup {

    Block* block1, *block2;
    Vect2 normal;

    Vect2 calc_normal();

public:

    CollisionGroup(Block* b1, Block* b2);

    void solve();

};
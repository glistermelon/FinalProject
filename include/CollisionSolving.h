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
    Point point;
    Vect2 normal;
    Vect2 r1, r2;

    double total_impulse = 0;

    Vect2 calc_rel_velocity();

    Vect2 calc_impulse(); // argument should be either point1 or point2

public:

    Collision(Block* b1, Block* b2, Point p, Vect2 normal);

    void solve();

};

class CollisionGroup {

    const unsigned int num_iterations = 5;

    Block* block1, *block2;
    Vect2 normal;
    std::vector<Collision> collisions;

    Vect2 calc_normal(bool& optimal);

public:

    CollisionGroup(Block* b1, Block* b2);

    void solve();

};
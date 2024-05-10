#pragma once

#include <list>

#include "Block.h"
#include "Vect2.h"

class Collision {

    Point point1, point2;
    Block* block1, *block2;

    double total_impulse = 0; // cumulative impulse

    static std::pair<Point, Point> calc_points();

    static Vect2 calc_normal();

    static double calc_impulse(Point p, Block* b); // impulse for point p which is on Block b

public:

    Collision(Block* b1, Block* b2);

    void solve();
    void apply();

};

class QuadtreeNode {

    QuadtreeNode *top_left = nullptr,
        *top_right = nullptr,
        *bottom_left = nullptr,
        *bottom_right = nullptr;

    std::list<Block*> blocks;

    bool are_colliding(Block* b1, Block* b2);

    void handle_collisions();

};
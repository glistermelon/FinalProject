#pragma once

#include <list>

#include "Block.h"
#include "Vect2.h"

class Collision {

    Point point1, point2;
    Vect2 normal;

    double total_impulse = 0; // cumulative impulse

    double calc_impulse(Point*);

public:

    Collision(Point p1, Point p2, Vect2 normal);

    void solve();

};

class CollisionGroup {

    Block* block1, *block2;
    Vect2 normal;

    Vect2 calc_normal();
    std::vector<Collision> find_collisions();

};

class QuadtreeNode {

    const static unsigned int split_threshold = 5;

    QuadtreeNode *top_left = nullptr,
        *top_right = nullptr,
        *bottom_left = nullptr,
        *bottom_right = nullptr;

    Rect bounds;

    std::vector<Block*> blocks;

    bool are_colliding(Block* b1, Block* b2);

public:

    void insert_block(Block*);
    void remove_block(Block*);

    void handle_collisions();

};
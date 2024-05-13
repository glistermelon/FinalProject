#pragma once

#include <list>

#include "Block.h"
#include "Vect2.h"

// https://box2d.org/files/ErinCatto_DynamicBVH_Full.pdf
class QuadtreeNode {

    QuadtreeNode *top_left = nullptr,
        *top_right = nullptr,
        *bottom_left = nullptr,
        *bottom_right = nullptr;

    Rect bounds;



public:

    void insert_block(Block*);
    void remove_block(Block*);

};
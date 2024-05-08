#pragma once

#include <list>

#include "Block.h"

class QuadtreeNode {

    QuadtreeNode *top_left = nullptr,
        *top_right = nullptr,
        *bottom_left = nullptr,
        *bottom_right = nullptr;

    std::list<Block*> blocks;

    bool are_colliding(Block* b1, Block* b2);

    // void update_block(Block*);

    // void handle_collisions();

};